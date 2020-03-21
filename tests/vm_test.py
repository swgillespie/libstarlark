import unittest
import starlark


class VmTests(unittest.TestCase):
  def test_smoke_create_vm(self):
    vm = starlark.VirtualMachine()