# Project 3 — Candy Shop Cashier

## Project

[Name] Karim Ehab Gamal
[Gmail] anakarimehab67@gmail.com

**Candy Shop Cashier**

This project simulates a candy shop cashier using plain C.

The program manages six kinds of candy, a shopping basket, customer payments, change, and an end-of-day sales report.

## How to Build

Compile the program with:

```bash
gcc -std=c99 -Wall -Wextra -o app main.c
```

Run it with:

```bash
./app
```

## Data

The shop contains six candy kinds.

Each candy has:
- Name
- Price in piastres
- Current stock
- Number sold today

The basket can contain up to eight lines. Each basket line stores a candy ID and its quantity.

The program also keeps the amount of money currently in the cash drawer.

## Main Functions

- `openShop()` — fills the shelf with six candies, their prices and stock, clears today's sold counts, and starts with an empty cash drawer.
- `showShelf()` — displays the candies, prices, and remaining stock.
- `addToBasket()` — adds a candy and quantity while checking that the candy exists, enough stock is available, and the basket has room.
- `removeFromBasket()` — removes a basket line and shifts later lines forward.
- `basketTotal()` — calculates the total cost of the basket.
- `showBasket()` — displays the basket as a receipt with quantities, prices, line totals, and the final total.
- `checkout()` — handles payment, checks that enough money was given, updates stock and sales, adds money to the drawer, gives change, and clears the basket after a successful purchase.
- `giveChange()` — calculates change using the available coin denominations.
- `bestSeller()` — returns the position of the candy with the highest number of sales.
- `dayReport()` — displays the cash drawer amount, total candies sold, best seller, and sold-out candies.

## Important Requirements

- Shelf stock must never become negative.
- If the customer does not pay enough, the basket must remain unchanged.
- Adding the same candy twice must create one basket line instead of two.
- The basket can contain at most eight lines.
- If a candy is sold out, the shelf should show `SOLD OUT`.
- Change should be returned using the fewest available coins: 500, 200, 100, 50, and 25 piastres.
- If the change is zero, the program should report that there is no change.

## Explain Why

### Why is 137 piastres impossible as change?

The available coin denominations are 500, 200, 100, 50, and 25 piastres.

All of these coin values are multiples of 25. Therefore, any amount that can be made using these coins must also be a multiple of 25.

137 is not a multiple of 25, so it cannot be represented using only these coins.

The program should therefore handle 137 piastres as an impossible change amount rather than pretending that the exact change can be returned.

## Screenshots

### 1. Fresh Start

[Fresh Start]

### 2. Interesting Action

[Interesting Action] & [Interesting Action (1)]

### 3. Candy Shop Report

[Candy Shop report]