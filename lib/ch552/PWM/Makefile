CFLAGS=

PWM.rel: PWM.c
	sdcc --model-small PWM.c -I ../../../include -c $(CFLAGS)


clean:
	-rm *.lst
	-rm *.sym
	-rm *.rel
	-rm *.asm
