echo gcc -Wall -O2 -o xcsi xcsi.c -lX11
gcc -Wall -O2 -o xcsi xcsi.c -lX11
echo gcc -Wall -O2 -o cbtest ClipboardTest.c Clipboard.c `allegro-config --libs release`
gcc -Wall -O2 -o cbtest ClipboardTest.c Clipboard.c `allegro-config --libs release`

