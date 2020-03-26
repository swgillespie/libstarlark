import unittest
import starlark


class VmTests(unittest.TestCase):
    def test_smoke_create_vm(self):
        vm = starlark.VirtualMachine()

    def test_bad_create_vm(self):
        with self.assertRaises(TypeError):
            starlark.VirtualMachine(42)

    def test_smoke_create_thread(self):
        vm = starlark.VirtualMachine()
        thread = starlark.Thread(vm)

    def test_smoke_bad_create_thread(self):
        with self.assertRaises(TypeError):
            starlark.Thread("not a vm")

class ThreadTests(unittest.TestCase):
    def setUp(self):
        self.vm = starlark.VirtualMachine()

    def test_slot_count_starts_at_zero(self):
        thread = starlark.Thread(self.vm)
        self.assertEqual(0, thread.slot_count())