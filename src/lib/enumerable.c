#include <slash.h>

static SLVAL
enumerable_map(sl_vm_t* vm, SLVAL self, SLVAL f)
{
    SLVAL array = sl_make_array(vm, 0, NULL);
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    SLVAL val;
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        val = sl_send(vm, f, "call", 1, val);
        sl_array_push(vm, array, 1, &val);
    }
    return array;
}

static SLVAL
enumerable_to_a(sl_vm_t* vm, SLVAL self)
{
    SLVAL array = sl_make_array(vm, 0, NULL);
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    SLVAL val;
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        sl_array_push(vm, array, 1, &val);
    }
    return array;
}

static SLVAL
enumerable_reduce(sl_vm_t* vm, SLVAL self, size_t argc, SLVAL* argv)
{
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    SLVAL acc, val, f;
    if(argc == 1) {
        if(!sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
            sl_throw_message2(vm, vm->lib.ArgumentError, "Can't reduce empty array without initializer");
        } else {
            acc = sl_send(vm, enumerator, "current", 0);
        }
        f = argv[0];
    } else {
        acc = argv[0];
        f = argv[1];
    }
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        acc = sl_send(vm, f, "call", 2, acc, val);
    }
    return acc;
}

SLVAL
sl_enumerable_join(sl_vm_t* vm, SLVAL self, size_t argc, SLVAL* argv)
{
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    SLVAL joiner, val, str;
    if(argc) {
        joiner = sl_to_s(vm, argv[0]);
    } else {
        joiner = sl_make_cstring(vm, "");
    }
    if(!sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        return sl_make_cstring(vm, "");
    }
    str = sl_to_s(vm, sl_send(vm, enumerator, "current", 0));
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        val = sl_to_s(vm, val);
        str = sl_string_concat(vm, str, joiner);
        str = sl_string_concat(vm, str, val);
    }
    return str;
}

static SLVAL
enumerable_length(sl_vm_t* vm, SLVAL self)
{
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    SLVAL i = sl_make_int(vm, 0);
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        i = sl_send(vm, i, "succ", 0);
    }
    return i;
}

static SLVAL
enumerable_empty(sl_vm_t* vm, SLVAL self)
{
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    if(!sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        return vm->lib._true;
    } else {
        return vm->lib._false;
    }
}

static SLVAL
enumerable_any(sl_vm_t* vm, SLVAL self, size_t argc, SLVAL* argv)
{
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    SLVAL val;
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        if(argc > 0) {
            val = sl_send(vm, argv[0], "call", 1, val);
        }
        if(sl_is_truthy(val)) {
            return vm->lib._true;
        }
    }
    return vm->lib._false;
}

static SLVAL
enumerable_all(sl_vm_t* vm, SLVAL self, size_t argc, SLVAL* argv)
{
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    SLVAL val;
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        if(argc > 0) {
            val = sl_send(vm, argv[0], "call", 1, val);
        }
        if(!sl_is_truthy(val)) {
            return vm->lib._false;
        }
    }
    return vm->lib._true;
}

static SLVAL
enumerable_find(sl_vm_t* vm, SLVAL self, SLVAL f)
{
    SLVAL val, truthy;
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        truthy = sl_send(vm, f, "call", 1, val);
        if(sl_is_truthy(truthy)) {
            return val;
        }
    }
    return vm->lib.nil;
}

static SLVAL
enumerable_filter(sl_vm_t* vm, SLVAL self, SLVAL f)
{
    SLVAL val, truthy, ary = sl_make_array(vm, 0, NULL);
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        truthy = sl_send(vm, f, "call", 1, val);
        if(sl_is_truthy(truthy)) {
            sl_array_push(vm, ary, 1, &val);
        }
    }
    return ary;
}

static SLVAL
enumerable_reject(sl_vm_t* vm, SLVAL self, SLVAL f)
{
    SLVAL val, truthy, ary = sl_make_array(vm, 0, NULL);
    SLVAL enumerator = sl_send(vm, self, "enumerate", 0);
    while(sl_is_truthy(sl_send(vm, enumerator, "next", 0))) {
        val = sl_send(vm, enumerator, "current", 0);
        truthy = sl_send(vm, f, "call", 1, val);
        if(!sl_is_truthy(truthy)) {
            sl_array_push(vm, ary, 1, &val);
        }
    }
    return ary;
}

static SLVAL
enumerable_sort(sl_vm_t* vm, SLVAL self)
{
    return sl_array_sort(vm, enumerable_to_a(vm, self));
}

void
sl_init_enumerable(sl_vm_t* vm)
{
    vm->lib.Enumerable = sl_define_class(vm, "Enumerable", vm->lib.Object);
    sl_define_method(vm, vm->lib.Enumerable, "map", 1, enumerable_map);
    sl_define_method(vm, vm->lib.Enumerable, "to_a", 0, enumerable_to_a);
    sl_define_method(vm, vm->lib.Enumerable, "reduce", -2, enumerable_reduce);
    sl_define_method(vm, vm->lib.Enumerable, "fold", -2, enumerable_reduce);
    sl_define_method(vm, vm->lib.Enumerable, "join", -1, sl_enumerable_join);
    sl_define_method(vm, vm->lib.Enumerable, "length", 0, enumerable_length);
    sl_define_method(vm, vm->lib.Enumerable, "empty", 0, enumerable_empty);
    sl_define_method(vm, vm->lib.Enumerable, "any", -1, enumerable_any);
    sl_define_method(vm, vm->lib.Enumerable, "all", -1, enumerable_all);
    sl_define_method(vm, vm->lib.Enumerable, "find", 1, enumerable_find);
    sl_define_method(vm, vm->lib.Enumerable, "filter", 1, enumerable_filter);
    sl_define_method(vm, vm->lib.Enumerable, "reject", 1, enumerable_reject);
    sl_define_method(vm, vm->lib.Enumerable, "sort", 0, enumerable_sort);
}
