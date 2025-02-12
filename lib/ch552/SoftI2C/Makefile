CFLAGS=


SoftI2C.rel: SoftI2C.c
	sdcc --model-small SoftI2C.c -I ../../../include -c $(CFLAGS)


clean:
	-rm *.lst
	-rm *.sym
	-rm *.rel
	-rm *.asm
