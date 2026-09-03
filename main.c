#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CANDY_KINDS   6U
#define BASKET_MAX    8U
#define NAME_LEN     16U

typedef struct {
    char     name[NAME_LEN];
    uint16_t price;      /* piastres */
    uint16_t stock;      /* how many are left on the shelf */
    uint16_t sold;       /* how many we sold today         */
} Candy_t;

typedef struct {
    uint8_t  candyId;
    uint8_t  qty;
} Line_t;

static Candy_t shelf[CANDY_KINDS];
static Line_t  basket[BASKET_MAX];
static uint8_t basketLines;      /* how many slots of the basket are used */
static uint32_t cashDrawer;

static void     openShop(void);
static void     showShelf(void);
static void     addToBasket(void);
static void     removeFromBasket(void);
static uint32_t basketTotal(void);
static void     showBasket(void);
static void     checkout(void);
static void     giveChange(uint32_t change);
static uint8_t  bestSeller(void);
static void     dayReport(void);
static void     openShop(void)
{
    static const Candy_t initialShelf[CANDY_KINDS] = {
        {"Toffee", 25, 20, 0}, {"Gummy", 50, 15, 0},
        {"Lollipop", 100, 10, 0}, {"Chocolate", 200, 12, 0},
        {"Caramel", 500, 8, 0}, {"Jelly Beans", 75, 18, 0}
    };

    memcpy(shelf, initialShelf, sizeof shelf);
    memset(basket, 0, sizeof basket);
    basketLines = 0;
    cashDrawer = 0;
}

static void showShelf(void)
{
    unsigned int index;

    printf("\nNo. %-15s Price  Stock\n", "Candy");
    for (index = 0; index < CANDY_KINDS; ++index) {
        printf("%2u. %-15s %5u  ", index + 1, shelf[index].name,
               shelf[index].price);
        if (shelf[index].stock == 0)
            printf("SOLD OUT\n");
        else
            printf("%5u\n", shelf[index].stock);
    }
}

static void addToBasket(void)
{
    unsigned int candyNumber, quantity, index;
    Candy_t *candy;

    printf("Candy number: ");
    if (scanf("%u", &candyNumber) != 1 || candyNumber < 1 ||
        candyNumber > CANDY_KINDS) {
        printf("That candy does not exist.\n");
        return;
    }
    candy = &shelf[candyNumber - 1];
    printf("Quantity: ");
    if (scanf("%u", &quantity) != 1 || quantity == 0 ||
        quantity > candy->stock) {
        printf("Not enough candy in stock.\n");
        return;
    }

    for (index = 0; index < basketLines; ++index) {
        if (basket[index].candyId == candyNumber - 1) {
            if ((unsigned int)basket[index].qty + quantity > 255U ||
                (unsigned int)basket[index].qty + quantity > candy->stock) {
                printf("That quantity cannot be added.\n");
                return;
            }
            basket[index].qty = (uint8_t)(basket[index].qty + quantity);
            printf("Added to basket.\n");
            return;
        }
    }
    if (basketLines == BASKET_MAX) {
        printf("The basket is full.\n");
        return;
    }
    basket[basketLines].candyId = (uint8_t)(candyNumber - 1);
    basket[basketLines].qty = (uint8_t)quantity;
    ++basketLines;
    printf("Added to basket.\n");
}

static uint32_t basketTotal(void)
{
    uint32_t total = 0;
    unsigned int index;

    for (index = 0; index < basketLines; ++index)
        total += (uint32_t)shelf[basket[index].candyId].price * basket[index].qty;
    return total;
}

static void showBasket(void)
{
    unsigned int index;

    printf("\n%-15s Qty  Each  Cost\n", "Candy");
    for (index = 0; index < basketLines; ++index) {
        Candy_t *candy = &shelf[basket[index].candyId];
        printf("%-15s %3u %5u %5u\n", candy->name, basket[index].qty,
               candy->price, candy->price * basket[index].qty);
    }
    printf("Total: %u\n", basketTotal());
}

static void removeFromBasket(void)
{
    unsigned int line, index;

    if (basketLines == 0) {
        printf("The basket is empty.\n");
        return;
    }
    showBasket();
    printf("Line to remove: ");
    if (scanf("%u", &line) != 1 || line < 1 || line > basketLines) {
        printf("That basket line does not exist.\n");
        return;
    }
    for (index = line - 1; index + 1 < basketLines; ++index)
        basket[index] = basket[index + 1];
    --basketLines;
    printf("Removed.\n");
}

static void giveChange(uint32_t change)
{
    const unsigned int coins[] = {500, 200, 100, 50, 25};
    unsigned int index, count;

    if (change == 0) {
        printf("No change, thank you.\n");
        return;
    }
    for (index = 0; index < sizeof coins / sizeof coins[0]; ++index) {
        count = change / coins[index];
        if (count != 0)
            printf("%u x %u coin(s)\n", count, coins[index]);
        change %= coins[index];
    }
}

static void checkout(void)
{
    uint32_t total, payment;
    unsigned int index;

    total = basketTotal();
    if (total == 0) {
        printf("The basket is empty.\n");
        return;
    }
    showBasket();
    printf("Money handed over: ");
    if (scanf("%u", &payment) != 1 || payment < total) {
        printf("That is not enough money.\n");
        return;
    }
    for (index = 0; index < basketLines; ++index) {
        shelf[basket[index].candyId].stock -= basket[index].qty;
        shelf[basket[index].candyId].sold += basket[index].qty;
    }
    cashDrawer += total;
    giveChange(payment - total);
    basketLines = 0;
    printf("Thank you!\n");
}

static uint8_t bestSeller(void)
{
    unsigned int index;
    uint8_t best = 0;

    for (index = 1; index < CANDY_KINDS; ++index)
        if (shelf[index].sold > shelf[best].sold)
            best = (uint8_t)index;
    return shelf[best].sold == 0 ? 0 : (uint8_t)(best + 1);
}

static void dayReport(void)
{
    uint32_t totalSold = 0;
    unsigned int index, best;

    for (index = 0; index < CANDY_KINDS; ++index)
        totalSold += shelf[index].sold;
    best = bestSeller();
    printf("\nCash drawer: %u\nCandies sold: %u\n", cashDrawer, totalSold);
    printf("Best seller: %s\n", best == 0 ? "None" : shelf[best - 1].name);
    printf("Sold out: ");
    for (index = 0; index < CANDY_KINDS; ++index)
        if (shelf[index].stock == 0)
            printf("%s ", shelf[index].name);
    printf("\n");
}

int main(void)
{
    unsigned int choice;

    openShop();
    do {
        printf("\n1. Shelf  2. Add  3. Remove  4. Basket  5. Checkout  6. Report  0. Exit\nChoice: ");
        if (scanf("%u", &choice) != 1)
            break;
        switch (choice) {
        case 1: showShelf(); break;
        case 2: addToBasket(); break;
        case 3: removeFromBasket(); break;
        case 4: showBasket(); break;
        case 5: checkout(); break;
        case 6: dayReport(); break;
        case 0: break;
        default: printf("Invalid choice.\n"); break;
        }
    } while (choice != 0);
    return 0;
}



