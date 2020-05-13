#pragma once
// Provide the basic rts.c functions that are used by the generated code:
__attribute__((noreturn)) static void sail_match_failure(sail_string msg) {
    fprintf(stderr, "Pattern match failure in %s\n", msg);
    abort();
}
unit sail_assert(bool b, sail_string msg) {
    if (b)
        return UNIT;
    fprintf(stderr, "Assertion failed: %s\n", msg);
    abort();
}

/* ***** Setup and cleanup functions for RTS ***** */
static int process_arguments(int argc, char** argv) { abort(); }
static void setup_rts(void) { setup_library(); }
static void cleanup_rts(void) { cleanup_library(); }

// this is needed for some reason:
bool have_exception = false;
