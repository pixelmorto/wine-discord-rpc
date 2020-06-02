#!/bin/bash
set -e

# based off doitsujin/dxvk setup_dxvk script

# where are we?
basedir=$(dirname "$(readlink -f $0)")

# what should we do?
action="$1"

case "$action" in
install)
	;;
uninstall)
	;;
*)
	echo "what?: $action"
	echo "usage: $0 [install/uninstall]"
	exit 2
esac

# check wine prefix before invoking wine, so that we
# don't accidentally create one if the user screws up
if [ -n "$WINEPREFIX" ] && ! [ -f "$WINEPREFIX/system.reg" ]; then
	echo "$WINEPREFIX: "'broken WINEPREFIX (use absolute paths!)' >&2
	exit 1
fi

# are we on wow64 or win32?
export WINEDEBUG=-all
wine_bin="wine"
wine64_bin="wine64"
wine_path=$(dirname "$(which wineboot)")

if [ ! -f "$wine_path/$wine_bin" ]; then
	wine_bin="$wine64_bin"
fi

wine_ver=$($wine_bin --version | grep wine)
if [ -z "$wine_ver" ]; then
	echo "$wine_bin: "'broken wine installation' >&2
	exit 1
fi

$wine_bin wineboot -u

# determine WINEPREFIX architecture
win_sys_path=$($wine_bin winepath -u 'C:\windows\system32' 2>/dev/null)
win_sys_path="${win_sys_path/$'\r'/}"
wow64_sys_path=$($wine_bin winepath -u 'C:\windows\syswow64' 2>/dev/null)
wow64_sys_path="${wow64_sys_path/$'\r'/}"

windows_dir=$($wine_bin winepath -u 'C:\windows' 2>/dev/null)
windows_dir="${windows_dir/$'\r'/}"

binsuf=win64

if [ -z "$wow64_sys_path" ] && [ -z "$win_sys_path" ]; then
	echo "$wine_bin: "'no system32 in WINEPREFIX?' >&2
	exit 1
fi

if [ -z "$wow64_sys_path" ]; then
	binsuf=win32
fi

install()
{
	ln -sfv "$basedir/wine-discord-rpc.$binsuf.exe.so" "$windows_dir/wine-discord-rpc.exe"
	$wine_bin reg add 'HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunServices' /v 'wine-discord-rpc' /d 'C:\windows\wine-discord-rpc.exe' /f >/dev/null 2>&1
}

uninstall()
{
	rm -v "$windows_dir/wine-discord-rpc.exe"
	$wine_bin reg delete 'HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunServices' /v 'wine-discord-rpc' /f >/dev/null 2>&1
}

$action
