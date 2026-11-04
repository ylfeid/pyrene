#!/bin/sh
fail=0

boot() {
    out=$(mktemp)
    qemu-system-aarch64 -machine virt -cpu cortex-a53 -smp 4 -m 512M \
        -nographic -kernel build/Image -append "$1" < /dev/null > "$out" 2>&1 &
    pid=$!
    sleep 4
    kill "$pid" 2>/dev/null
    wait "$pid" 2>/dev/null
}

check() {
    name=$1
    shift
    for pat in "$@"; do
        if ! grep -q -- "$pat" "$out"; then
            echo "FAIL ($name): missing '$pat'"
            fail=1
        fi
    done
    cat "$out"
    rm -f "$out"
}

boot ""
check boot "pyrene booting" "pl011 @ 0x0000000009000000" "cpus: *4" "(512 MiB)"

boot "selftest=brk"
check brk "bootargs: *selftest=brk" "exception: sync (el1h)" "ec 0x3c brk, iss 0x0000042" \
    "x30: " "KERNEL PANIC: unhandled exception"

if [ "$fail" -eq 0 ]; then
    echo "itest: PASS"
fi
exit "$fail"
