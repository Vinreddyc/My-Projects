set $lastcs = -1

define hook-stop
  # There doesn't seem to be a good way to detect if we're in 16- or
  # 32-bit mode, but we always run with CS == 8 in 32-bit mode.
  if $cs == 8 || $cs == 27
    if $lastcs != 8 && $lastcs != 27
      set architecture i386
    end
    x/i $pc
  else
    if $lastcs == -1 || $lastcs == 8 || $lastcs == 27
      set architecture i8086
    end
    # Translate the segment:offset into a physical address
    printf "[%4x:%4x] ", $cs, $eip
    x/i $cs*16+$eip
  end
  set $lastcs = $cs
end

echo + target remote localhost:4444\n
target remote localhost:4444

# If this fails, it's probably because your GDB doesn't support ELF.
# Look at the tools page at
#  http://pdos.csail.mit.edu/6.828/2009/tools.html
# for instructions on building GDB with ELF support.
echo + symbol-file kern/init/kernel\n
symbol-file boot/boot0/boot0.elf
add-symbol-file boot/boot1/boot1.elf 0x7e00
add-symbol-file kern/init/kernel 0x100000