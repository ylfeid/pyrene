#!/bin/sh
fail=0
out=$(mktemp)
trap 'rm -f "$out"' EXIT
symbolizer=${LLVM:+$LLVM/}llvm-symbolizer

boot() {
    qemu-system-aarch64 -machine virt -cpu cortex-a53 -smp 4 -m 512M \
        -nographic -kernel build/Image -append "$1" < /dev/null > "$out" 2>&1 &
    pid=$!
    sleep 4
    kill "$pid" 2>/dev/null
    wait "$pid" 2>/dev/null
    cat "$out"
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
}

boot ""
check boot "pyrene booting" "pl011 @ 0x0000000009000000" "cpus: *4" "(512 MiB)"

boot "selftest=brk"
check brk "bootargs: *selftest=brk" "exception: sync (el1h)" "ec 0x3c brk, iss 0x0000042" \
    "x30: " "backtrace:" "KERNEL PANIC: unhandled exception"

pcs=""
for a in $(sed -n '/backtrace:/,/^$/p' "$out" | grep -o '0x[0-9a-f]*'); do
    [ -n "$pcs" ] && a=$(printf '0x%x' $((a - 4)))
    pcs="$pcs $a"
done
frames=$("$symbolizer" --obj=build/kernel.elf --functions=short $pcs | \
    grep -v -e : -e '^$' | tr '\n' ' ')
echo "backtrace symbols: $frames"
for fn in selftest_brk kernel_main; do
    case " $frames " in
    *" $fn "*) ;;
    *) echo "FAIL (brk): '$fn' not in backtrace"; fail=1 ;;
    esac
done

if [ "$fail" -eq 0 ]; then
    echo "itest: PASS"
fi
exit "$fail"
