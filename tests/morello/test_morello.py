#!/usr/bin/env python3
import pytest
import os
import subprocess
import sys
import typing
from pathlib import Path

qemu_args = "-machine morello -nographic -kernel".split()

@pytest.fixture
def qemu_binary(request) -> Path:
    return request.config.getoption("--qemu")

@pytest.fixture
def tests_dir(request) -> Path:
    result = Path(request.config.getoption("--morello-tests-dir")).absolute()
    if not result.is_dir():
        sys.exit("--morello-tests-dir is not a directory")
    return result


def find_morello_tests():
    should_fail_list = [
        "2020-10-01-test5/test16.elf",
        "2020-10-07-test5/test15.elf",
        "2020-10-07-test8/test46.elf"
    ]
    ignore_list = [
        # These access SP before writing to it
        "2020-10-01-test5/test44.elf",
        "2020-10-07-test5/test42.elf",
        # These use different sizes/registers/addresses for LDX/STX
        "2020-11-10-generated-tests/test-03-012.elf",
        "2020-11-10-generated-tests/test-04-023.elf",
        "2020-11-26-test-bundle/new-tests/test-ldxp-stlxp_r_cr_c-002.elf",
    ]
    # noinspection PyUnresolvedReferences
    tests_dir = pytest.morello_tests_dir  # type: Path
    assert tests_dir.is_dir()
    for root, dirs, files in os.walk(str(tests_dir)):
        # Only walk dirs that are named "*test*"
        # However, there is also "2021-02-06-exn-tests/bad-tests/" which
        # should probably not be run.
        dirs = [d for d in dirs if "test" in d]
        dirs.sort()
        if dirs:
            print("Test dirs=", dirs)

        for file in sorted(files):
            if file.endswith('.elf'):
                path = Path(root, file)
                fullpath = str(path)
                dirname = path.parent.name
                should_print_failed = dirname == "failing-tests"
                marks = ()  # type: typing.Any
                if dirname in ("failing-tests", "bad-tests"):
                    # TODO: see which ones should pass
                    marks = (pytest.mark.skip(reason="Inside " + dirname + "folder"),)
                elif any(fullpath.endswith(name) for name in ignore_list):
                    marks = (pytest.mark.skip(),)
                elif any(fullpath.endswith(name) for name in should_fail_list):
                    marks = (pytest.mark.xfail(strict=True),)
                yield pytest.param(path, should_print_failed,
                                   id=os.path.relpath(fullpath[:-4], tests_dir),
                                   marks=marks)

@pytest.mark.parametrize("elf_file,should_print_failed", find_morello_tests())
def test_morello_elf_file(elf_file, should_print_failed: bool, qemu_binary):
    command = [str(qemu_binary), *qemu_args, elf_file]
    # Timeout will fail the test
    sp = subprocess.run(command, capture_output=True, timeout=5)
    code = sp.returncode
    result = sp.stdout
    print(sp)
    lines = result.split(b'\n')
    status = None
    if len(lines) > 0:
        line = lines[-2]
        if line.endswith(b'OK'):
            status = True
        if line.endswith(b'FAILED'):
            status = False
    if status is None:
        pytest.fail("Did not get a result from the test")
    elif status is True and should_print_failed:
        pytest.fail("Did not get a failure message from the test")
    elif status is False and not should_print_failed:
        pytest.fail("Got an unexpected failure message from the test")
    assert code != 0 or status is True
    return status


if __name__ == "__main__":
    pytest.main()
