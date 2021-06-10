import os
import pytest
from pathlib import Path

def abspath(x):
    return Path(x).absolute()

def pytest_addoption(parser):
    morello_root_dir = os.getenv("MORELLO_ROOT", str(Path.home() / "cheri"))
    parser.addoption("--qemu", type=abspath, help="Path to qemu-system-morello",
                     default=str(Path(morello_root_dir, "output/morello-sdk/bin/qemu-system-morello")))
    parser.addoption("--morello-tests-dir", type=abspath, help="Path to the morello tests", required=True)


# noinspection PyUnresolvedReferences
def pytest_configure(config):
    pytest.morello_tests_dir = config.getoption("--morello-tests-dir")
    print("Morello tests dir is", pytest.morello_tests_dir)
    pytest.morello_qemu = config.getoption("--qemu")
    print("Morello QEMU is", pytest.morello_qemu)
