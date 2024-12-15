#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
int getChoice();
void addCar();
void listCars();
void searchCar(int registration);
void deleteCar();
int modifyMenu();
void modifyRegistration();
void modifyBrand();
void modifyPrice();

// Structure to represent a car
typedef struct {
    int registration;
    char brand[50];
    float price;
} Car;

Car car;
FILE *file;
int mainChoice = 0;
int modifyChoice = 0;

int main() {
    // User authentication
    char users[2][10] = {"kbaaalla", "asmatt"};
    char login[10];
    int access_granted = 0;
    int i;

    printf("Enter your username: ");
    fgets(login, sizeof(login), stdin);
    login[strcspn(login, "\n")] = '\0'; 

    for (i = 0; i < 2; i++) {
        if (strcmp(users[i], login) == 0) {
            access_granted = 1; // User found
            break;
        }
    }

    if (!access_granted) {
        printf("Access denied! You are not authorized to use this program.\n");
        return 0; // Exit the program if access is denied
    }

    printf("Welcome, %s!\n\n", login);

   
    while (mainChoice != 6) {
        mainChoice = getChoice();
        switch (mainChoice) {
            case 1:
                addCar();
                break;
            case 2:
                listCars();
                break;
            case 3: {
                int reg;
                printf("Enter the registration number of the car to search: ");
                scanf("%d", &reg);
                searchCar(reg);
                break;
            }
            case 4: {
                while (modifyChoice != 4) {
                    modifyChoice = modifyMenu();
                    switch (modifyChoice) {
                        case 1:
                            modifyRegistration();
                            break;
                        case 2:
                            modifyBrand();
                            break;
                        case 3:
                            modifyPrice();
                            break;
                        case 4:
                            printf("Modification canceled.\n");
                            break;
                    }
                }
                modifyChoice = 0; 
                break;
            }
            case 5:
                deleteCar();
                break;
            case 6:
                printf("Goodbye! Thank you for using the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    return 0;
}

// Function to display the main menu
int getChoice() {
    printf("\n***** MENU *****\n");
    printf("1 ---> Add a car\n");
    printf("2 ---> Display the list of cars\n");
    printf("3 ---> Search for a car by registration number\n");
    printf("4 ---> Modify a car\n");
    printf("5 ---> Delete a car\n");
    printf("6 ---> Quit\n");
    printf("Your choice: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

// Function to add a new car
void addCar() {
    printf("Enter registration number: ");
    scanf("%d", &car.registration);
    printf("Enter brand: ");
    scanf("%s", car.brand);
    printf("Enter price: ");
    scanf("%f", &car.price);

    file = fopen("cars.txt", "ab"); 
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fwrite(&car, sizeof(car), 1, file);
    fclose(file);

    printf("Car added successfully!\n");
}


void listCars() {
    file = fopen("cars.txt", "rb");
    if (file == NULL) {
        printf("No cars available.\n");
        return;
    }

    printf("\nList of cars:\n");
    while (fread(&car, sizeof(car), 1, file)) {
        printf("Registration Number: %d\n", car.registration);
        printf("Brand: %s\n", car.brand);
        printf("Price: %.2f\n\n", car.price);
    }
    fclose(file);
}

// Function to search for a car by registration number
void searchCar(int registration) {
    int found = 0;
    file = fopen("cars.txt", "rb");
    if (file == NULL) {
        printf("No cars available to search.\n");
        return;
    }

    while (fread(&car, sizeof(car), 1, file)) {
        if (car.registration == registration) {
            printf("\nCar found:\n");
            printf("Registration Number: %d\n", car.registration);
            printf("Brand: %s\n", car.brand);
            printf("Price: %.2f\n\n", car.price);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Car with registration number %d not found.\n", registration);
    }
}


void deleteCar() {
    int regToDelete;
    FILE *tempFile;

    printf("Enter the registration number of the car to delete: ");
    scanf("%d", &regToDelete);

    file = fopen("cars.txt", "rb");
    tempFile = fopen("temp.txt", "wb");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while (fread(&car, sizeof(car), 1, file)) {
        if (car.registration != regToDelete) {
            fwrite(&car, sizeof(car), 1, tempFile);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);

    remove("cars.txt");
    rename("temp.txt", "cars.txt");

    if (found) {
        printf("Car deleted successfully.\n");
    } else {
        printf("Car with registration number %d not found.\n", regToDelete);
    }
}


int modifyMenu() {
    printf("\n***** MODIFICATION MENU *****\n");
    printf("1 ---> Modify registration number\n");
    printf("2 ---> Modify brand\n");
    printf("3 ---> Modify price\n");
    printf("4 ---> Cancel modification\n");
    printf("Your choice: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}


void modifyRegistration() {
    int oldReg, newReg;
    FILE *tempFile;

    printf("Enter the registration number to modify: ");
    scanf("%d", &oldReg);
    printf("Enter the new registration number: ");
    scanf("%d", &newReg);

    file = fopen("cars.txt", "rb");
    tempFile = fopen("temp.txt", "wb");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&car, sizeof(car), 1, file)) {
        if (car.registration == oldReg) {
            car.registration = newReg;
        }
        fwrite(&car, sizeof(car), 1, tempFile);
    }
    fclose(file);
    fclose(tempFile);

    remove("cars.txt");
    rename("temp.txt", "cars.txt");

    printf("Registration number updated successfully.\n");
}

// Function to modify the brand
void modifyBrand() {
    int reg;
    char newBrand[50];
    FILE *tempFile;

    printf("Enter the registration number of the car to modify: ");
    scanf("%d", &reg);
    printf("Enter the new brand: ");
    scanf("%s", newBrand);

    file = fopen("cars.txt", "rb");
    tempFile = fopen("temp.txt", "wb");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&car, sizeof(car), 1, file)) {
        if (car.registration == reg) {
            strcpy(car.brand, newBrand);
        }
        fwrite(&car, sizeof(car), 1, tempFile);
    }
    fclose(file);
    fclose(tempFile);

    remove("cars.txt");
    rename("temp.txt", "cars.txt");

    printf("Brand updated successfully.\n");
}

// Function to modify the price
void modifyPrice() {
    int reg;
    float newPrice;
    FILE *tempFile;

    printf("Enter the registration number of the car to modify: ");
    scanf("%d", &reg);
    printf("Enter the new price: ");
    scanf("%f", &newPrice);

    file = fopen("cars.txt", "rb");
    tempFile = fopen("temp.txt", "wb");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&car, sizeof(car), 1, file)) {
        if (car.registration == reg) {
            car.price = newPrice;
        }
        fwrite(&car, sizeof(car), 1, tempFile);
    }
    fclose(file);
    fclose(tempFile);

    remove("cars.txt");
    rename("temp.txt", "cars.txt");

    printf("Price updated successfully.\n");
}
