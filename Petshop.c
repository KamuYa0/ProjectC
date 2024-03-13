#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PRODUCTS 10


struct ExpiryDate {
    int year;
    int month;
};

struct Product {
    char name[50];
    int price;
    int quantity;
    struct ExpiryDate expiry_date;
};

int checkExpiryDate(struct ExpiryDate expiry_date) {
    time_t now;
    struct tm *info;
    time(&now);
    info = localtime(&now);

    int current_year = info->tm_year + 1900;
    int current_month = info->tm_mon + 1;

    if (expiry_date.year < current_year || (expiry_date.year == current_year && expiry_date.month < current_month)) {
        return 1; // Product has expired
    } else {
        return 0; // Product has not expired
    }
}

void searchProduct(struct Product products[], int num_products) {
    char search_name[50];
    int found = 0;

    if (num_products > 0) {
        printf("Enter the name of the product you want to search for: ");
        scanf("%s", search_name);

        // Convert search_name to lowercase
        for (int i = 0; search_name[i]; i++) {
            search_name[i] = tolower(search_name[i]);
        }

        printf("Search Results:\n");
        printf("---------------------------------------------------------------\n");
        for (int i = 0; i < num_products; i++) {
            char temp_name[50];
            strcpy(temp_name, products[i].name);

            // Convert product name to lowercase
            for (int j = 0; temp_name[j]; j++) {
                temp_name[j] = tolower(temp_name[j]);
            }

            if (strstr(temp_name, search_name) != NULL) {
                printf("Product: %s - Price: %d - Quantity: %d - Expiry Date: %d/%d\n", products[i].name, products[i].price, products[i].quantity, products[i].expiry_date.month, products[i].expiry_date.year);
                found = 1;
            }
        }
        if (!found) {
            printf("Product not found.\n");
        }
        printf("---------------------------------------------------------------\n");
    } else {
        printf("No products available for search.\n");
    }
}

void addProduct(struct Product products[], int *num_products) {
    if (*num_products < MAX_PRODUCTS) {
        printf("Enter product name: ");
        getchar(); // Clear input buffer
        fgets(products[*num_products].name, sizeof(products[*num_products].name), stdin);
        products[*num_products].name[strlen(products[*num_products].name) - 1] = '\0'; 
        printf("Enter product price: ");
        scanf("%d", &products[*num_products].price);
        printf("Enter product quantity: ");
        scanf("%d", &products[*num_products].quantity);
        printf("Enter product expiry year: ");
        scanf("%d", &products[*num_products].expiry_date.year);
        printf("Enter product expiry month: ");
        scanf("%d", &products[*num_products].expiry_date.month);
        (*num_products)++;
        printf("Product added successfully.\n");
    } else {
        printf("Maximum number of products reached.\n");
    }
}

void deleteProduct(struct Product products[], int *num_products) {
    if (*num_products > 0) {
        int delete_index;
        printf("Enter the index of the product you want to delete: ");
        scanf("%d", &delete_index);

        if (delete_index >= 0 && delete_index < *num_products) {
            for (int i = delete_index; i < *num_products - 1; i++) {
                products[i] = products[i + 1];
            }
            (*num_products)--;
            printf("Product deleted successfully.\n");
        } else {
            printf("Invalid index.\n");
        }
    } else {
        printf("No products to delete.\n");
    }
}

void showAllProducts(struct Product products[], int num_products) {
    if (num_products > 0) {
        printf("All Products:\n");
        printf("---------------------------------------------------------------\n");
        for (int i = 0; i < num_products; i++) {
            printf("Product %d: %s - Price: %d - Quantity: %d - Expiry Date: %d/%d\n", i+1, products[i].name, products[i].price, products[i].quantity, products[i].expiry_date.month, products[i].expiry_date.year);
        }
        printf("---------------------------------------------------------------\n");
    } else {
        printf("No products available.\n");
    }
}

void editProduct(struct Product products[], int num_products) {
    if (num_products > 0) {
        int edit_index;
        printf("Enter the index of the product you want to edit: ");
        scanf("%d", &edit_index);

        if (edit_index >= 0 && edit_index < num_products) {
            printf("Enter new product name: ");
            getchar(); // Clear input buffer
            fgets(products[edit_index].name, sizeof(products[edit_index].name), stdin);
            products[edit_index].name[strlen(products[edit_index].name) - 1] = '\0'; // Remove new line character from fgets
            printf("Enter new product price: ");
            scanf("%d", &products[edit_index].price);
            printf("Enter new product quantity: ");
            scanf("%d", &products[edit_index].quantity);
            printf("Enter new product expiry year: ");
            scanf("%d", &products[edit_index].expiry_date.year);
            printf("Enter new product expiry month: ");
            scanf("%d", &products[edit_index].expiry_date.month);

            printf("Product information updated successfully.\n");
        } else {
            printf("Invalid index.\n");
        }
    } else {
        printf("No products to edit.\n");
    }
}

void sortProductsByName(struct Product products[], int num_products) {
    struct Product temp;
    int i, j;

    for (i = 0; i < num_products - 1; i++) {
        for (j = 0; j < num_products - i - 1; j++) {
            if (strcmp(products[j].name, products[j + 1].name) > 0) {
                temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }

    printf("Products sorted by name.\n");
}

void saveProductsToFile(struct Product products[], int num_products) {
    FILE *fp;
    fp = fopen("products.txt", "w");

    if (fp == NULL) {
        printf("Error opening or creating file.\n");
        return;
    }

    for (int i = 0; i < num_products; i++) {
        fprintf(fp, "%s,%d,%d,%d,%d\n", products[i].name, products[i].price, products[i].quantity, products[i].expiry_date.year, products[i].expiry_date.month);
    }

    fclose(fp);
    printf("Products saved to file successfully.\n");
}

void loadProductsFromFile(struct Product products[], int *num_products) {
    FILE *fp;
    fp = fopen("products.txt", "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (*num_products < MAX_PRODUCTS && fscanf(fp, "%49[^,],%d,%d,%d,%d\n", products[*num_products].name, &products[*num_products].price, &products[*num_products].quantity, &products[*num_products].expiry_date.year, &products[*num_products].expiry_date.month) == 5) {
        (*num_products)++;
    }

    fclose(fp);
    printf("Products loaded from file successfully.\n");
}

void checkAndNotifyExpiry(struct Product products[], int num_products) {
    FILE *fp;
    fp = fopen("expiry_notification.txt", "w");

    if (fp == NULL) {
        printf("Error opening or creating file.\n");
        return;
    }

    time_t now;
    struct tm *info;
    time(&now);
    info = localtime(&now);

    int current_year = info->tm_year + 1900;
    int current_month = info->tm_mon + 1;

    fprintf(fp, "Expiry Notification:\n\n");
    for (int i = 0; i < num_products; i++) {
        if (checkExpiryDate(products[i].expiry_date)) {
            fprintf(fp, "Product %s has expired.\n", products[i].name);
        } else {
            fprintf(fp, "Product %s has not expired.\n", products[i].name);
        }
    }

    fclose(fp);
    printf("Expiry notification saved to file successfully.\n");
}

int main() {
    struct Product products[MAX_PRODUCTS];
    int num_products = 0;
    loadProductsFromFile(products, &num_products);
    checkAndNotifyExpiry(products, num_products);

    int action;
    do {
        printf("\nPet product list\n\n");
        printf("╔═════════════════════════════╗\n");
        printf("║ Choose an action:           ║\n");
        printf("║ 1 - Add a product           ║\n");
        printf("║ 2 - Delete a product        ║\n");
        printf("║ 3 - Show all products       ║\n");
        printf("║ 4 - Edit a product          ║\n");
        printf("║ 5 - Search for a product    ║\n");
        printf("║ 6 - Sort products by name   ║\n");
        printf("║ 9 - Quit                    ║\n");
        printf("╚═════════════════════════════╝\n");

        scanf("%d", &action);
        switch(action) {
            case 1: // Add a product
                addProduct(products, &num_products);
                break;

            case 2: // Delete a product
                deleteProduct(products, &num_products);
                break;

            case 3: // Show all products
                showAllProducts(products, num_products);
                break;

            case 4: // Edit a product
                editProduct(products, num_products);
                break;

            case 5: // Search for a product
                searchProduct(products, num_products);
                break;

            case 6: // Sort products by name
                sortProductsByName(products, num_products);
                break;

            case 9: // Quit
                saveProductsToFile(products, num_products);
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid action. Please try again.\n");
        }
    } while(action != 9);

    return 0;
}
