/*
 * Core Definitions for QAPI/QMP Dispatch
 *
 * Copyright IBM, Corp. 2011
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.1 or later.
 * See the COPYING.LIB file in the top-level directory.
 *
 */

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qapi/qmp/dispatch.h"
#include "qapi/qmp/json-parser.h"
#include "qapi/qmp/qdict.h"
#include "qapi/qmp/qjson.h"
#include "qapi/qmp/qbool.h"
#include "sysemu/sysemu.h"

QDict *qmp_dispatch_check_obj(const QObject *request, Error **errp)
{
    const QDictEntry *ent;
    const char *arg_name;
    const QObject *arg_obj;
    bool has_exec_key = false;
    QDict *dict = NULL;

    dict = qobject_to(QDict, request);
    if (!dict) {
        error_setg(errp, "QMP input must be a JSON object");
        return NULL;
    }

    for (ent = qdict_first(dict); ent;
         ent = qdict_next(dict, ent)) {
        arg_name = qdict_entry_key(ent);
        arg_obj = qdict_entry_value(ent);

        if (!strcmp(arg_name, "execute")) {
            if (qobject_type(arg_obj) != QTYPE_QSTRING) {
                error_setg(errp,
                           "QMP input member 'execute' must be a string");
                return NULL;
            }
            has_exec_key = true;
        } else if (!strcmp(arg_name, "arguments")) {
            if (qobject_type(arg_obj) != QTYPE_QDICT) {
                error_setg(errp,
                           "QMP input member 'arguments' must be an object");
                return NULL;
            }
        } else if (!strcmp(arg_name, "id")) {
            continue;
        } else if (!strcmp(arg_name, "control")) {
            if (qobject_type(arg_obj) != QTYPE_QDICT) {
                error_setg(errp,
                           "QMP input member 'control' must be a dict");
                return NULL;
            }
        } else {
            error_setg(errp, "QMP input member '%s' is unexpected",
                       arg_name);
            return NULL;
        }
    }

    if (!has_exec_key) {
        error_setg(errp, "QMP input lacks member 'execute'");
        return NULL;
    }

    return dict;
}

static QObject *do_qmp_dispatch(QmpCommandList *cmds, QObject *request,
                                Error **errp)
{
    Error *local_err = NULL;
    const char *command;
    QDict *args, *dict;
    QmpCommand *cmd;
    QObject *ret = NULL;

    dict = qmp_dispatch_check_obj(request, errp);
    if (!dict) {
        return NULL;
    }

    command = qdict_get_str(dict, "execute");
    cmd = qmp_find_command(cmds, command);
    if (cmd == NULL) {
        error_set(errp, ERROR_CLASS_COMMAND_NOT_FOUND,
                  "The command %s has not been found", command);
        return NULL;
    }
    if (!cmd->enabled) {
        error_setg(errp, "The command %s has been disabled for this instance",
                   command);
        return NULL;
    }

    if (runstate_check(RUN_STATE_PRECONFIG) &&
        !(cmd->options & QCO_ALLOW_PRECONFIG)) {
        error_setg(errp, "The command '%s' isn't permitted in '%s' state",
                   cmd->name, RunState_str(RUN_STATE_PRECONFIG));
        return NULL;
    }

    if (!qdict_haskey(dict, "arguments")) {
        args = qdict_new();
    } else {
        args = qdict_get_qdict(dict, "arguments");
        qobject_ref(args);
    }

    cmd->fn(args, &ret, &local_err);
    if (local_err) {
        error_propagate(errp, local_err);
    } else if (cmd->options & QCO_NO_SUCCESS_RESP) {
        g_assert(!ret);
    } else if (!ret) {
        ret = QOBJECT(qdict_new());
    }

    qobject_unref(args);

    return ret;
}

QObject *qmp_build_error_object(Error *err)
{
    return qobject_from_jsonf("{ 'class': %s, 'desc': %s }",
                              QapiErrorClass_str(error_get_class(err)),
                              error_get_pretty(err));
}

/*
 * Detect whether a request should be run out-of-band, by quickly
 * peeking at whether we have: { "control": { "run-oob": true } }. By
 * default commands are run in-band.
 */
bool qmp_is_oob(QDict *dict)
{
    QBool *bool_obj;

    dict = qdict_get_qdict(dict, "control");
    if (!dict) {
        return false;
    }

    bool_obj = qobject_to(QBool, qdict_get(dict, "run-oob"));
    if (!bool_obj) {
        return false;
    }

    return qbool_get_bool(bool_obj);
}

QObject *qmp_dispatch(QmpCommandList *cmds, QObject *request)
{
    Error *err = NULL;
    QObject *ret;
    QDict *rsp;

    ret = do_qmp_dispatch(cmds, request, &err);

    rsp = qdict_new();
    if (err) {
        qdict_put_obj(rsp, "error", qmp_build_error_object(err));
        error_free(err);
    } else if (ret) {
        qdict_put_obj(rsp, "return", ret);
    } else {
        qobject_unref(rsp);
        return NULL;
    }

    return QOBJECT(rsp);
}
