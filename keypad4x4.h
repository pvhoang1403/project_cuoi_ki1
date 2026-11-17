#ifndef KEYPAD4X4_H
#define KEYPAD4X4_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

/* ------------------------------- Gan chan -------------------------------- */
/* Neu CubeMX da dat User Label R1..R4 / C1..C4 (main.h co R1_Pin...) -> dung
 * label. Neu chua -> dung truc tiep PB8..PB11 (hang) va PB12..PB15 (cot).   */
#ifndef KP_R1_PORT
  #if defined(R1_Pin) && defined(R4_Pin)
    #define KP_R1_PORT   R1_GPIO_Port
    #define KP_R1_PIN    R1_Pin
    #define KP_R2_PORT   R2_GPIO_Port
    #define KP_R2_PIN    R2_Pin
    #define KP_R3_PORT   R3_GPIO_Port
    #define KP_R3_PIN    R3_Pin
    #define KP_R4_PORT   R4_GPIO_Port
    #define KP_R4_PIN    R4_Pin
  #else
    #define KP_R1_PORT   GPIOB
    #define KP_R1_PIN    GPIO_PIN_8
    #define KP_R2_PORT   GPIOB
    #define KP_R2_PIN    GPIO_PIN_9
    #define KP_R3_PORT   GPIOB
    #define KP_R3_PIN    GPIO_PIN_10
    #define KP_R4_PORT   GPIOB
    #define KP_R4_PIN    GPIO_PIN_11
  #endif
#endif

#ifndef KP_C1_PORT
  #if defined(C1_Pin) && defined(C4_Pin)
    #define KP_C1_PORT   C1_GPIO_Port
    #define KP_C1_PIN    C1_Pin
    #define KP_C2_PORT   C2_GPIO_Port
    #define KP_C2_PIN    C2_Pin
    #define KP_C3_PORT   C3_GPIO_Port
    #define KP_C3_PIN    C3_Pin
    #define KP_C4_PORT   C4_GPIO_Port
    #define KP_C4_PIN    C4_Pin
  #else
    #define KP_C1_PORT   GPIOB
    #define KP_C1_PIN    GPIO_PIN_12
    #define KP_C2_PORT   GPIOB
    #define KP_C2_PIN    GPIO_PIN_13
    #define KP_C3_PORT   GPIOB
    #define KP_C3_PIN    GPIO_PIN_14
    #define KP_C4_PORT   GPIOB
    #define KP_C4_PIN    GPIO_PIN_15
  #endif
#endif

/* ----------------------------- Bo cuc phim ------------------------------- */
#define KEYPAD_LAYOUT_CALC   0   /* keypad Proteus KEYPAD-SMALLCALC:
                                    7 8 9 /   4 5 6 *   1 2 3 -   C 0 = +    */
#define KEYPAD_LAYOUT_STD    1   /* keypad 4x4 thong dung (nhu code goc):
                                    1 2 3 A   4 5 6 B   7 8 9 C   * 0 # D    */
#ifndef KEYPAD_LAYOUT
#define KEYPAD_LAYOUT        KEYPAD_LAYOUT_STD
#endif

/* ------------------------------ Tuy chon --------------------------------- */
#define KEYPAD_NO_KEY              0x01   /* gia tri tra ve khi khong bam (nhu code goc) */

#ifndef KEYPAD_DEBOUNCE_MS
#define KEYPAD_DEBOUNCE_MS         20U    /* chong doi khi bam / khi nha              */
#endif

#ifndef KEYPAD_RELEASE_TIMEOUT_MS
#define KEYPAD_RELEASE_TIMEOUT_MS  2000U  /* toi da cho nha phim (ms); 0 = cho mai.
                                             Co timeout -> khong bi treo khi cot
                                             tha noi / phim ket.                  */
#endif

/* ------------------------------- API ------------------------------------- */
void Keypad_Init(void);           /* dua tat ca hang ve CAO                   */
char read_keypad(void);           /* quet 1 lan: ky tu phim, hoac KEYPAD_NO_KEY */
char Keypad_WaitKey(void);        /* cho den khi co phim duoc bam roi nha     */

/* Kiem tra phan cung: tat ca hang CAO, khong bam phim -> doc 4 cot.
 * Tra ve bitmask cac cot dang THAP (bit0 = C1 ... bit3 = C4).
 * Binh thuong phai = 0. Khac 0 -> cot tha noi (thieu pull-up) hoac phim ket. */
uint8_t Keypad_ColLowMask(void);

#ifdef __cplusplus
}
#endif

#endif /* KEYPAD_H */
