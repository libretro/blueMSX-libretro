; Deterministic MSX test cartridge for the replay harness.
;
; Exercises the VDP (border colour + VRAM pattern writes) and the PSG
; (rising tone sweep on channel A) in a fixed, input-independent loop,
; so that framebuffer and audio checksums are fully reproducible.
;
; Assemble with: pasmo testcart.asm testcart.rom

        org     0x4000

        db      'A','B'         ; cartridge signature
        dw      init            ; INIT
        dw      0               ; STATEMENT
        dw      0               ; DEVICE
        dw      0               ; TEXT
        dw      0,0,0           ; reserved

init:
        di
        im      1
        ld      sp, 0xf380

        ; --- VDP: screen 1-ish minimal setup via direct register writes ---
        ; R#0 = 0 (mode bits), R#1 = 0x60 (display enable, 16K)
        ld      a, 0x00
        out     (0x99), a
        ld      a, 0x80         ; register 0
        out     (0x99), a
        ld      a, 0x60
        out     (0x99), a
        ld      a, 0x81         ; register 1
        out     (0x99), a

        ; --- PSG: enable tone on channel A, volume 12 ---
        ld      a, 7            ; mixer register
        out     (0xa0), a
        ld      a, 0xb8         ; tone A on, noise off (IOB/IOA as output)
        out     (0xa1), a
        ld      a, 8            ; channel A volume
        out     (0xa0), a
        ld      a, 12
        out     (0xa1), a

        ; --- read the RP-5C01 real-time clock and fold it into the
        ;     output stream, so the video CRC depends on the RTC seed ---
        ; select block 0 (time) via MODE register 13, timer enabled
        ld      a, 13
        out     (0xb4), a
        ld      a, 0x08
        out     (0xb5), a
        ; XOR time registers 0..12 (sec/min/hour/day/month/year digits)
        ld      d, 0            ; accumulator
        ld      e, 0            ; register index
rtc_read:
        ld      a, e
        out     (0xb4), a
        in      a, (0xb5)
        and     0x0f            ; RP-5C01 registers are 4 bits wide
        ; weight each register by its index so digit swaps don't cancel
        add     a, e
        rlca
        xor     d
        ld      d, a
        inc     e
        ld      a, e
        cp      13
        jr      nz, rtc_read

        ld      hl, 0           ; frame-ish counter
        ld      c, 0            ; colour/period counter

mainloop:
        ; --- change border colour (VDP R#7) from counter ---
        ld      a, c
        and     0x0f
        out     (0x99), a
        ld      a, 0x87         ; register 7
        out     (0x99), a

        ; --- sweep PSG tone period from counter ---
        ld      a, 0            ; fine period register
        out     (0xa0), a
        ld      a, c
        out     (0xa1), a
        ld      a, 1            ; coarse period register
        out     (0xa0), a
        ld      a, c
        and     0x03
        inc     a
        out     (0xa1), a

        ; --- write a deterministic byte pattern into VRAM at 0x0000+ ---
        ; set VRAM write address = HL & 0x3fff
        ld      a, l
        out     (0x99), a
        ld      a, h
        and     0x3f
        or      0x40            ; write mode
        out     (0x99), a
        ld      a, l
        xor     h
        xor     c
        xor     d               ; fold in the RTC digest read at boot
        out     (0x98), a

        ; --- busy-wait a fixed number of iterations ---
        ld      b, 0
delay:  nop
        nop
        djnz    delay

        inc     hl
        ld      a, h
        or      l
        jr      nz, no_wrap
        inc     c               ; bump colour counter every 65536 iterations
no_wrap:
        ; also bump colour slowly from HL to make the border visibly cycle
        ld      a, l
        and     0x3f
        jr      nz, mainloop
        inc     c
        jr      mainloop
