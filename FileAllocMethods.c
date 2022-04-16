#include<stdio.h>

#define TOTAL_BLOCKS 32
#define TOTAL_FILES 5

void contiguousAllocation();
void linkedAllocation();
void indexedAllocation();

int main() {
    printf("\nMENU\n");
    printf("1. Contiguous Allocation\n");
    printf("2. Linked Allocation\n");
    printf("3. Indexed Allocation\n");
    printf("Enter choice: ");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        contiguousAllocation();
        break;
    case 2:
        linkedAllocation();
        break;
    case 3: 
        indexedAllocation();
        break;
    default:
        break;
    }
    return 0;
}

//Display disk blocks
void display(int blocks[]) {
    printf("\n\n");
    printf("Disk\n");
    for(int i=0;i<=(TOTAL_BLOCKS/4)-1;i++) {
        for(int j=0;j<4;j++) {
            printf("%2c ", blocks[4*i + j]);
        }
        printf("\n");
    }
    printf("\n");
}

struct ContiguousDirectoryEntry {
    char filename[20];
    int start;
    int length;
};

void contiguousAllocation() {
    int diskBlocks[TOTAL_BLOCKS];
    struct ContiguousDirectoryEntry directoryEntry[TOTAL_FILES];   // Index represents start index, value represents length
    for(int i=0;i<TOTAL_BLOCKS;i++) {
        diskBlocks[i] = '-';
    }

    int directoryEntryIndex = 0;

    for(int f=0;f<TOTAL_FILES;f++) {
        char filename[20];
        int b;
        int length;
        char input[TOTAL_BLOCKS];
        printf("File %d\n", f+1);
        printf("Enter filename: ");
        scanf("%s", filename);
        printf("Enter start index: ");
        scanf("%d", &b);
        printf("Enter length: ");
        scanf("%d", &length);
        printf("Enter file data: ");
        scanf("%s", input);

        if(b+length > TOTAL_BLOCKS) {
            printf("Cannot add file as input is out of range!\n\n");
            continue;
        }

        int occupied = 0; // false
        for(int i=b;i<b+length;i++) {       // Checks if any space is occupied 
            if(diskBlocks[i] != '-') {
                occupied = 1;               // Disk space already occupied
                break;
            }
        }
        if(occupied) {
            printf("Cannot add file as disk space already occupied!\n\n");
        } else {
            for(int i=b;i<b+length;i++) {
                diskBlocks[i] = input[i-b];      // Occupied
            }
            directoryEntry[directoryEntryIndex].start = b;
            directoryEntry[directoryEntryIndex].length = length; // Updated directory entry
            int i=0;
            for(i=0;filename[i]!='\0';i++) {
                directoryEntry[directoryEntryIndex].filename[i] = filename[i];
            }
            directoryEntry[directoryEntryIndex].filename[i] = '\0';
            

            directoryEntryIndex++;
            printf("File added");
            display(diskBlocks);
        }
    }
    printf("Entries\n");
    printf("\nFile\tStart\tLength\n\n");
    for(int i=0;i<TOTAL_FILES;i++) {
        printf("%s\t%d\t%d\n", directoryEntry[i].filename, directoryEntry[i].start, directoryEntry[i].length);
    }

    int start;
    int length = 0;
    int found = 0;
    printf("\nEnter file start location of file in disk: ");
    scanf("%d", &start);
    for(int i=0;i<TOTAL_FILES;i++) {
        if(directoryEntry[i].start == start) {
            found = 1;
            length = directoryEntry[i].length;
            break;
        }
    }
    if(found) {
        printf("File data: ");
        for(int i=start;i<start+length;i++) {
            printf("%c",diskBlocks[i]);
        }
    } else {
        printf("No such file in directory entry!");
    }
}

struct LinkedDirectoryEntry {
    char filename[20];
    int start;
    int end;
};

struct File {
    int fileData;
    int next;
};

void displayL(struct File blocks[]) {
    printf("\n\n");
    printf("Disk\n");
    for(int i=0;i<=(TOTAL_BLOCKS/4)-1;i++) {
        for(int j=0;j<4;j++) {
            printf("%2c ", blocks[4*i + j].fileData);
        }
        printf("\n");
    }
    printf("\n");
}

void linkedAllocation() {
    struct File diskBlocks[TOTAL_BLOCKS];
    struct LinkedDirectoryEntry directoryEntry[TOTAL_FILES];   // Index represents start index, value represents length
    for(int i=0;i<TOTAL_BLOCKS;i++) {
        diskBlocks[i].fileData = '-';
    }

    int directoryEntryIndex = 0;

    for(int f=0;f<TOTAL_FILES;f++) {
        char filename[20];
        int length;
        int b;
        char input[TOTAL_BLOCKS];
        printf("File %d\n", f+1);
        printf("Enter filename: ");
        scanf("%s", filename);
        printf("Enter start index: ");
        scanf("%d", &b);
        printf("Enter length: ");
        scanf("%d", &length);
        printf("Enter file data: ");
        scanf("%s", input);

        if(diskBlocks[b].fileData != '-') {
            printf("Cannot add file as start position already occupied!\n\n");
            directoryEntry[directoryEntryIndex].start = -1;
            directoryEntry[directoryEntryIndex].end = -1;
            continue;
        }

        int totalFreeSpace = 0;
        for(int i=0;i<TOTAL_BLOCKS;i++) {
            if(diskBlocks[i].fileData == '-') {
                totalFreeSpace++;
            }
        }

        if(totalFreeSpace<length) {
            printf("Cannot add file as not enough space available\n\n");
        } else {
            int k = length;
            int prev = b;
            diskBlocks[b].fileData = input[0];
            int i = 0;
            for(i=1;i<k;i++) {
                if(diskBlocks[i].fileData!='-') {
                    k++;
                    continue;
                }
                diskBlocks[i].fileData = input[i-k+length];      // Occupied
                diskBlocks[prev].next = i;
                prev = i;
            }
            directoryEntry[directoryEntryIndex].start = b;
            directoryEntry[directoryEntryIndex].end = i-1;
            i=0;
            for(i=0;filename[i]!='\0';i++) {
                directoryEntry[directoryEntryIndex].filename[i] = filename[i];
            }
            directoryEntry[directoryEntryIndex].filename[i] = '\0';

            directoryEntryIndex++;
            printf("File added");
            displayL(diskBlocks);
        }
    }
    printf("Entries\n");
    printf("\nFile\tStart\tEnd\n\n");
    for(int i=0;i<directoryEntryIndex;i++) {
        printf("%s\t%d\t%d\n", directoryEntry[i].filename, directoryEntry[i].start, directoryEntry[i].end);
    }
    int start;
    int end;
    int found = 0;
    printf("\nEnter file start location of file in disk: ");
    scanf("%d", &start);
    for(int i=0;i<directoryEntryIndex;i++) {
        if(directoryEntry[i].start == start) {
            found = 1;
            end = directoryEntry[i].end;
            break;
        }
    }
    if(found) {
        printf("File data: ");
        int i = start;
        while(i!=end) {
            printf("%c",diskBlocks[i],i);
            i = diskBlocks[i].next;
        }
        printf("%c",diskBlocks[end],i);
    } else {
        printf("No such file in directory entry!");
    }
}

struct IndexedDirectoryEntry {
    char filename[20];
    int indexedBlock;
};

struct IndexedFile {
    int fileData;
    int blocks[TOTAL_BLOCKS];
    int length;
};

void displayI(struct IndexedFile blocks[]) {
    printf("\n\n");
    printf("Disk\n");
    for(int i=0;i<=(TOTAL_BLOCKS/4)-1;i++) {
        for(int j=0;j<4;j++) {
            printf("%2c ", blocks[4*i + j].fileData);
        }
        printf("\n");
    }
    printf("\n");
}

void indexedAllocation() {
    struct IndexedFile diskBlocks[TOTAL_BLOCKS];
    struct IndexedDirectoryEntry directoryEntry[TOTAL_FILES];   // Index represents start index, value represents length
    for(int i=0;i<TOTAL_BLOCKS;i++) {
        diskBlocks[i].fileData = '-';
    }

    int directoryEntryIndex = 0;

    for(int f=0;f<TOTAL_FILES;f++) {
        char filename[20];
        int length;
        int b;
        char input[TOTAL_BLOCKS];
        printf("File %d\n", f+1);
        printf("Enter filename: ");
        scanf("%s", filename);
        printf("Enter indexed block index: ");
        scanf("%d", &b);
        printf("Enter length: ");
        scanf("%d", &length);
        printf("Enter file data: ");
        scanf("%s", input);

        if(diskBlocks[b].fileData != '-') {
            printf("Cannot add file as start position already occupied!\n\n");
            directoryEntry[directoryEntryIndex].indexedBlock = -1;
            continue;
        }

        int totalFreeSpace = 0;
        for(int i=0;i<TOTAL_BLOCKS;i++) {
            if(diskBlocks[i].fileData == '-') {
                totalFreeSpace++;
            }
        }

        if(totalFreeSpace<length+1) {
            printf("Cannot add file as not enough space available\n\n");
        } else {
            int k = length;
            int itr = 0;
            diskBlocks[b].fileData = '~';
            diskBlocks[b].length = length;
            int i = 0;
            for(i=0;i<k;i++) {
                if(diskBlocks[i].fileData!='-') {
                    k++;
                    continue;
                }
                diskBlocks[i].fileData = input[i-k+length];      // Occupied
                diskBlocks[b].blocks[itr++] = i;
            }
            directoryEntry[directoryEntryIndex].indexedBlock = b;
            i=0;
            for(i=0;filename[i]!='\0';i++) {
                directoryEntry[directoryEntryIndex].filename[i] = filename[i];
            }
            directoryEntry[directoryEntryIndex].filename[i] = '\0';

            directoryEntryIndex++;
            printf("File added");
            displayI(diskBlocks);
        }
    }
    printf("Entries\n");
    printf("\nFile\tIndex Block\n\n");
    for(int i=0;i<directoryEntryIndex;i++) {
        printf("%s\t%d\n", directoryEntry[i].filename, directoryEntry[i].indexedBlock);
    }
    int index;
    int found = 0;
    printf("\nEnter file index block location of file in disk: ");
    scanf("%d", &index);
    for(int i=0;i<directoryEntryIndex;i++) {
        if(directoryEntry[i].indexedBlock == index) {
            found = 1;
            break;
        }
    }
    if(found) {
        printf("File data: ");
        for(int i=0;i<diskBlocks[index].length;i++) {
            printf("%c", diskBlocks[diskBlocks[index].blocks[i]].fileData);
        }
    } else {
        printf("No such file in directory entry!");
    }
}