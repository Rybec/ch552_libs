
CFLAGS=


WS2812.rel: WS2812.c
	sdcc --model-small WS2812.c -I ../../../include -c $(CFLAGS)


clean:
	-rm *.lst
	-rm *.sym
	-rm *.rel
	-rm *.asm
