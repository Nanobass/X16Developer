.include "imag.inc"

.section .text.SaveContext
.global SaveContext
SaveContext:
  ; Save return address
  tsx
  lda $101,x
  ldy #0
  sta (__rc2),y
  lda $102,x
  iny
  sta (__rc2),y

  ; Save hard stack pointer
  txa
  iny
  sta (__rc2),y

  ; Save hard stack
  ; tay
  ldy #$0
SaveContext_loop:
  lda $100,y
  sta (__rc4),y
  iny
  bne SaveContext_loop
  
  ldy #$2

  ; Save soft stack pointer
  lda __rc0
  iny
  sta (__rc2),y
  lda __rc1
  iny
  sta (__rc2),y

  ; Save CSRs
  lda __rc18
  iny
  sta (__rc2),y
  lda __rc19
  iny
  sta (__rc2),y
  lda __rc20
  iny
  sta (__rc2),y
  lda __rc21
  iny
  sta (__rc2),y
  lda __rc22
  iny
  sta (__rc2),y
  lda __rc23
  iny
  sta (__rc2),y
  lda __rc24
  iny
  sta (__rc2),y
  lda __rc25
  iny
  sta (__rc2),y
  lda __rc26
  iny
  sta (__rc2),y
  lda __rc27
  iny
  sta (__rc2),y
  lda __rc28
  iny
  sta (__rc2),y
  lda __rc29
  iny
  sta (__rc2),y
  lda __rc30
  iny
  sta (__rc2),y
  lda __rc31
  iny
  sta (__rc2),y

  ; Return zero
  lda #0
  ldx #0
  rts

.section .text.LoadContext
.global LoadContext
LoadContext:
  ; Save return value

  ; Restore CSRs
  ldy #18
  lda (__rc2),y
  sta __rc31
  dey
  lda (__rc2),y
  sta __rc30
  dey
  lda (__rc2),y
  sta __rc29
  dey
  lda (__rc2),y
  sta __rc28
  dey
  lda (__rc2),y
  sta __rc27
  dey
  lda (__rc2),y
  sta __rc26
  dey
  lda (__rc2),y
  sta __rc25
  dey
  lda (__rc2),y
  sta __rc24
  dey
  lda (__rc2),y
  sta __rc23
  dey
  lda (__rc2),y
  sta __rc22
  dey
  lda (__rc2),y
  sta __rc21
  dey
  lda (__rc2),y
  sta __rc20
  dey
  lda (__rc2),y
  sta __rc19
  dey
  lda (__rc2),y
  sta __rc18

  ; Restore soft stack pointer
  dey
  lda (__rc2),y
  sta __rc1
  dey
  lda (__rc2),y
  sta __rc0

  ; Restore hard stack pointer
  dey
  lda (__rc2),y
  tax
  txs

  ; Restore hard stack
  ; tay
  ldy #$0
LoadContext_loop:
  lda (__rc4),y
  sta $100,y
  iny
  bne LoadContext_loop

  ldy #$1

  ; Restore return address
  lda (__rc2),y
  sta $102,x
  dey
  lda (__rc2),y
  sta $101,x

  ; Restore return value
  lda #1
  ldx #0
  rts