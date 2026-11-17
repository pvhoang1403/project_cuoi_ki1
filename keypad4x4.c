#include "keypad4x4.h"

/* ------------------------------ Bang chan -------------------------------- */
static GPIO_TypeDef * const row_port[4] = { KP_R1_PORT, KP_R2_PORT, KP_R3_PORT, KP_R4_PORT };
static const uint16_t       row_pin [4] = { KP_R1_PIN,  KP_R2_PIN,  KP_R3_PIN,  KP_R4_PIN  };
static GPIO_TypeDef * const col_port[4] = { KP_C1_PORT, KP_C2_PORT, KP_C3_PORT, KP_C4_PORT };
static const uint16_t       col_pin [4] = { KP_C1_PIN,  KP_C2_PIN,  KP_C3_PIN,  KP_C4_PIN  };

/* ------------------------------ Bang phim -------------------------------- */
static const char keymap[4][4] =
{
#if (KEYPAD_LAYOUT == KEYPAD_LAYOUT_STD)
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
#else   /* KEYPAD_LAYOUT_CALC - keypad trong so do Proteus */
    { '7', '8', '9', '/' },
    { '4', '5', '6', '*' },
    { '1', '2', '3', '-' },
    { 'C', '0', '=', '+' }      /* 'C' = ON/C */
#endif
};

/* ============================ Ham noi bo ================================= */

/* Keo hang 'r' xuong THAP, cac hang khac CAO. r >= 4 -> tat ca CAO. */
static void Keypad_Select_Row(uint8_t r)
{
    for (uint8_t i = 0; i < 4U; i++)
    {
        HAL_GPIO_WritePin(row_port[i], row_pin[i], (i == r) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
    for (volatile uint32_t d = 0; d < 20U; d++) { }   /* cho muc chan on dinh */
}

static inline uint8_t Keypad_Col_Low(uint8_t c)
{
    return (HAL_GPIO_ReadPin(col_port[c], col_pin[c]) == GPIO_PIN_RESET) ? 1U : 0U;
}

/* Cho nha phim o cot 'c' (co timeout tuy chon), roi chong doi khi nha */
static void Keypad_Wait_Release(uint8_t c)
{
    uint32_t t0 = HAL_GetTick();

    while (Keypad_Col_Low(c))
    {
#if (KEYPAD_RELEASE_TIMEOUT_MS > 0U)
        if ((HAL_GetTick() - t0) >= KEYPAD_RELEASE_TIMEOUT_MS) break;
#endif
    }
    (void)t0;
    HAL_Delay(KEYPAD_DEBOUNCE_MS);
}

/* ================================ API ==================================== */

/* Bat clock cho port GPIO */
static void Keypad_GPIO_ClkEnable(GPIO_TypeDef *port)
{
#ifdef GPIOA
    if (port == GPIOA) { __HAL_RCC_GPIOA_CLK_ENABLE(); return; }
#endif
#ifdef GPIOB
    if (port == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); return; }
#endif
#ifdef GPIOC
    if (port == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); return; }
#endif
#ifdef GPIOD
    if (port == GPIOD) { __HAL_RCC_GPIOD_CLK_ENABLE(); return; }
#endif
}

/* Tu cau hinh GPIO -> khong phu thuoc CubeMX da khai bao chan hay chua:
 * hang = output push-pull (muc CAO), cot = input pull-up */
void Keypad_Init(void)
{
    GPIO_InitTypeDef g = {0};

    for (uint8_t i = 0; i < 4U; i++)
    {
        Keypad_GPIO_ClkEnable(row_port[i]);
        HAL_GPIO_WritePin(row_port[i], row_pin[i], GPIO_PIN_SET);
        g.Pin   = row_pin[i];
        g.Mode  = GPIO_MODE_OUTPUT_PP;
        g.Pull  = GPIO_NOPULL;
        g.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(row_port[i], &g);

        Keypad_GPIO_ClkEnable(col_port[i]);
        g.Pin   = col_pin[i];
        g.Mode  = GPIO_MODE_INPUT;
        g.Pull  = GPIO_PULLUP;
        HAL_GPIO_Init(col_port[i], &g);
    }

    Keypad_Select_Row(0xFFU);          /* tat ca hang CAO */
}

char read_keypad(void)
{
    for (uint8_t r = 0; r < 4U; r++)
    {
        Keypad_Select_Row(r);          /* hang r THAP, cac hang khac CAO */

        for (uint8_t c = 0; c < 4U; c++)
        {
            if (Keypad_Col_Low(c))                 /* cot c THAP -> co the dang bam */
            {
                HAL_Delay(KEYPAD_DEBOUNCE_MS);     /* chong doi khi bam */
                if (!Keypad_Col_Low(c)) continue;  /* chi la nhieu -> bo qua */

                Keypad_Wait_Release(c);            /* cho nha phim */
                Keypad_Select_Row(0xFFU);          /* tra tat ca hang ve CAO */
                return keymap[r][c];
            }
        }
    }

    Keypad_Select_Row(0xFFU);
    return KEYPAD_NO_KEY;                          /* khong co phim nao */
}

uint8_t Keypad_ColLowMask(void)
{
    uint8_t mask = 0;
    Keypad_Select_Row(0xFFU);                      /* tat ca hang CAO */
    for (uint8_t c = 0; c < 4U; c++)
    {
        if (Keypad_Col_Low(c)) mask |= (uint8_t)(1U << c);
    }
    return mask;
}

char Keypad_WaitKey(void)
{
    char key;
    do
    {
        key = read_keypad();
    } while (key == KEYPAD_NO_KEY);
    return key;
}

