#include <bits/stdc++.h>

using namespace std;

const char DELETE_FLAG = '*';
const char DELIMITER = '|';
const short FREE = -2;

struct PIndex {
    char key[15]{};
    short RNN = -1;

    bool operator<(const PIndex &other) const {
        return strcmp(key, other.key) < 0;
    }
};

struct SIndex {
    char key[30]{};
    short RNN;

    bool operator<(const SIndex &other) const {
        return strcmp(key, other.key) < 0;
    }
};

struct LabelIDList {
    char primaryIndex[15]{};
    short RNN;
};

struct Author {
    char ID[15]{}; // primary key
    char name[30]{}; // secondary key
    char address[30]{};

    string format() {
        return string(ID) + "|" + string(name) + "|" + string(address);
    }

    void parse(string s) {
        int i = 0;
        for (; s[i] != DELIMITER; i++);
        strcpy(ID, s.substr(0, i).c_str());
        i++;
        int old = i;
        for (; s[i] != DELIMITER; i++);
        strcpy(name, s.substr(old, i - old).c_str());
        i++;
        strcpy(address, s.substr(i).c_str());
    }

    friend ostream &operator<<(ostream &out, const Author &author) {
        out << "Author ID: " << author.ID << "\n" << "Author Name: " << author.name << "\n" << "Author Address: "
            << author.address;
        return out;
    }

    friend istream &operator>>(istream &in, Author &author) {
        string input;
        cout << "Enter Author ID: " << endl;
        cin >> input;
        strcpy(author.ID, input.c_str());
        cout << "Enter Author Name: " << endl;
        cin >> input;
        strcpy(author.name, input.c_str());
        cout << "Enter Author Address: " << endl;
        cin >> input;
        strcpy(author.address, input.c_str());
        return in;
    }
};

struct Book {
    char ISBN[15]{}; // primary key
    char authorID[15]{}; // secondary key
    char title[30]{};

    string format() {
        return string(ISBN) + "|" + string(authorID) + "|" + string(title);
    }

    void parse(string s) {
        int i = 0;
        for (; s[i] != DELIMITER; i++);
        strcpy(ISBN, s.substr(0, i).c_str());
        i++;
        int old = i;
        for (; s[i] != DELIMITER; i++);
        strcpy(authorID, s.substr(old, i - old).c_str());
        i++;
        strcpy(title, s.substr(i).c_str());
    }

    friend ostream &operator<<(ostream &out, const Book &book) {
        out << "ISBN: " << book.ISBN << "\n" << "Author ID: " << book.authorID << "\n" << "Title: " << book.title;
        return out;
    }

    friend istream &operator>>(istream &in, Book &book) {
        string input;
        cout << "Enter ISBN: " << endl;
        cin >> input;
        strcpy(book.ISBN, input.c_str());
        cout << "Enter Author ID: " << endl;
        cin >> input;
        strcpy(book.authorID, input.c_str());
        cout << "Enter Title: " << endl;
        cin >> input;
        strcpy(book.title, input.c_str());
        return in;
    }
};

int menu() {
    cout << "1. Add New Author" << endl;
    cout << "2. Add New Book" << endl;
    cout << "3. Update Author Name (Author ID)" << endl;
    cout << "4. Update Book Title (ISBN)" << endl;
    cout << "5. Delete Book (ISBN)" << endl;
    cout << "6. Delete Author (Author ID)" << endl;
    cout << "7. Print Author (Author ID)" << endl;
    cout << "8. Print Book (ISBN)" << endl;
    cout << "9. Write Query" << endl;
    cout << "10. Exit" << endl;
    cout << "Enter your choice: " << endl;
    int choice;
    cin >> choice;
    return choice;
}

string formatDeletedRecord(short next, short space) {
    string s = DELETE_FLAG + to_string(next) + DELIMITER + to_string(space);
    return s;
}

pair<short, short> parseDeletedRecord(string s) {
    short next = 0, space = 0;
    int i = 1;
    for (; s[i] != DELIMITER; i++);
    next = stoi(s.substr(1, i));
    space = stoi(s.substr(i + 1));
    return {next, space};
}

void writePrimaryIndex(PIndex primaryIndexArray[], short numRec, fstream &outFile) {
    for (int i = 0; i < numRec; i++)
        outFile.write((char *) &primaryIndexArray[i], sizeof primaryIndexArray[i]);
}

void writeSecondaryIndex(SIndex secondIndexArray[], short numRec, fstream &outFile) {
    for (int i = 0; i < numRec; i++)
        outFile.write((char *) &secondIndexArray[i], sizeof secondIndexArray[i]);
}

void writeLabelIDList(LabelIDList labelIDListArray[], short numRec, fstream &outFile) {
    for (int i = 0; i < numRec; i++)
        outFile.write((char *) &labelIDListArray[i], sizeof labelIDListArray[i]);
}

void readPrimaryIndex(PIndex primaryIndexArray[], short numRec, fstream &inFile) {
    for (int i = 0; i < numRec; i++)
        inFile.read((char *) &primaryIndexArray[i], sizeof primaryIndexArray[i]);
}

void readSecondaryIndex(SIndex secondIndexArray[], short numRec, fstream &inFile) {
    for (int i = 0; i < numRec; i++)
        inFile.read((char *) &secondIndexArray[i], sizeof secondIndexArray[i]);
}

void readLabelIDList(LabelIDList labelIDListArray[], short numRec, fstream &inFile) {
    for (int i = 0; i < numRec; i++)
        inFile.read((char *) &labelIDListArray[i], sizeof labelIDListArray[i]);
}


int primaryIndexSearch(PIndex primaryIndex[], short numRec, string &key) {
    int l = 0, r = numRec - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (strcmp(primaryIndex[mid].key, key.c_str()) == 0) {
            return mid;
        }
        if (strcmp(primaryIndex[mid].key, key.c_str()) > 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return -1;
}

int secondaryIndexSearch(SIndex secondaryIndex[], short numRec, string &key) {
    int l = 0, r = numRec - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (strcmp(secondaryIndex[mid].key, key.c_str()) == 0) {
            return mid;
        }
        if (strcmp(secondaryIndex[mid].key, key.c_str()) > 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return -1;
}

void checkAvailList(fstream &file, short &RNN, short len) {
    short AvailList;
    file.seekg(0, ios::beg);
    file.read((char *) &AvailList, sizeof(AvailList));
    short prev = -1, prevSpace = -1;
    while (~AvailList) {
        // read the deleted record
        file.seekg(AvailList, ios::beg);
        short deletedRecordLen = 0;
        file.read((char *) &deletedRecordLen, sizeof(deletedRecordLen));
        char deletedRecord[deletedRecordLen + 1];
        file.read(deletedRecord, deletedRecordLen);
        deletedRecord[deletedRecordLen] = '\0';
        pair<short, short> deletedRecordInfo = parseDeletedRecord(string(deletedRecord));

        // check if the deleted record is large enough
        if (deletedRecordInfo.second >= len) {
            if (prev == -1) { // if this was the first record update avail list
                file.seekg(0, ios::beg);
                file.write((char *) &deletedRecordInfo.first, sizeof(deletedRecordInfo.second));
            } else { // if this was not the first record update the next of the previous record
                file.seekg(prev, ios::beg);
                string prevDeletedRecord = formatDeletedRecord(deletedRecordInfo.first, prevSpace);
                char prevDeletedRecordChar[prevDeletedRecord.size() + 1];
                strcpy(prevDeletedRecordChar, prevDeletedRecord.c_str());
                prevDeletedRecordChar[prevDeletedRecord.size()] = '\0';
                auto prevDeletedRecordLen = (short) prevDeletedRecord.size();
                file.write((char *) &prevDeletedRecordLen, sizeof(prevDeletedRecordLen));
                file.write(prevDeletedRecordChar, prevDeletedRecordLen);
            }
            // if it's large enough then update the RNN and the put pointer
            file.seekp(AvailList, ios::beg);
            RNN = AvailList;
            break;
        }
        prevSpace = deletedRecordInfo.second;
        prev = AvailList;
        AvailList = deletedRecordInfo.first;
    }
}

void updateSecondaryIndex(string fileName, SIndex secondaryIndex[], short secondaryIndexNumRec,
                          fstream &secondaryIndexFile) {
    sort(secondaryIndex, secondaryIndex + secondaryIndexNumRec);
    secondaryIndexFile.open(fileName.c_str(), ios::out | ios::trunc);
    secondaryIndexFile.seekp(0, ios::beg);
    secondaryIndexFile.write((char *) &secondaryIndexNumRec, sizeof(secondaryIndexNumRec));
    writeSecondaryIndex(secondaryIndex, secondaryIndexNumRec, secondaryIndexFile);
    secondaryIndexFile.close();
}

void updateLabelIDList(string fileName, LabelIDList labelIDList[], short labelIDListNumRec, fstream &labelIDListFile) {
    labelIDListFile.open(fileName, ios::out | ios::trunc);
    labelIDListFile.seekp(0, ios::beg);
    labelIDListFile.write((char *) &labelIDListNumRec, sizeof(labelIDListNumRec));
    writeLabelIDList(labelIDList, labelIDListNumRec, labelIDListFile);
    labelIDListFile.close();
}

void updatePrimaryIndex(string fileName, PIndex primaryIndex[], short primaryIndexNumRec, fstream &primaryIndexFile) {
    sort(primaryIndex, primaryIndex + primaryIndexNumRec);
    primaryIndexFile.open(fileName.c_str(), ios::out | ios::trunc);
    primaryIndexFile.seekp(0, ios::beg);
    writePrimaryIndex(primaryIndex, primaryIndexNumRec, primaryIndexFile);
    primaryIndexFile.close();
}

void addToPrimaryIndex(string fileName, short numRec, char key[], short RNN) {
    fstream primaryIndexFile(fileName.c_str(), ios::in | ios::out | ios::binary);
    PIndex primaryIndex[numRec + 1];
    primaryIndexFile.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndex, numRec, primaryIndexFile);
    strcpy(primaryIndex[numRec].key, key);
    primaryIndex[numRec].RNN = RNN;
    sort(primaryIndex, primaryIndex + numRec + 1);
    primaryIndexFile.close();
    // update the primary index file
    updatePrimaryIndex(fileName, primaryIndex, numRec + 1, primaryIndexFile);
}

void addToSecondaryIndex(string secondaryFileName, string labelFileName, char secondaryKey[], char primaryKey[]) {
    fstream secondaryIndexFile(secondaryFileName.c_str(), ios::in | ios::out | ios::binary);
    short secondaryIndexNumRec;
    secondaryIndexFile.seekg(0, ios::beg);
    secondaryIndexFile.read((char *) &secondaryIndexNumRec, sizeof(secondaryIndexNumRec));
    SIndex secondaryIndex[secondaryIndexNumRec + 1];
    readSecondaryIndex(secondaryIndex, secondaryIndexNumRec, secondaryIndexFile);
    secondaryIndexFile.close();
    string authorName = secondaryKey;
    int index = secondaryIndexSearch(secondaryIndex, secondaryIndexNumRec, authorName);
    if (index == -1) {
        // if the author name is not in the secondary index
        // add it to the secondary index
        strcpy(secondaryIndex[secondaryIndexNumRec].key, authorName.c_str());
        secondaryIndex[secondaryIndexNumRec].RNN = -1;
        index = secondaryIndexNumRec;
        secondaryIndexNumRec++;
    }
    // add the record to the label id list
    fstream labelIDListFile(labelFileName.c_str(), ios::in | ios::out | ios::binary);
    short labelIDListNumRec;
    labelIDListFile.seekg(0, ios::beg);
    labelIDListFile.read((char *) &labelIDListNumRec, sizeof(labelIDListNumRec));
    LabelIDList labelIDList[labelIDListNumRec + 1];
    readLabelIDList(labelIDList, labelIDListNumRec, labelIDListFile);
    labelIDListFile.close();
    short i = 0;
    for (; i <= labelIDListNumRec; i++) {
        if (labelIDList[i].RNN == FREE || i == labelIDListNumRec) {
            strcpy(labelIDList[i].primaryIndex, primaryKey);
            labelIDList[i].RNN = secondaryIndex[index].RNN;
            secondaryIndex[index].RNN = i;
            break;
        }
    }
    labelIDListNumRec += (i == labelIDListNumRec);
    updateSecondaryIndex(secondaryFileName, secondaryIndex, secondaryIndexNumRec, secondaryIndexFile);
    updateLabelIDList(labelFileName, labelIDList, labelIDListNumRec, labelIDListFile);
}

void removeRecordFromSecondaryIndex(SIndex secondaryIndex[], LabelIDList labelIDList[], short secondaryIndexNumRec,
                                    char key[], string &secondaryKey) {
    // delete the record with the old name
    string Key = string(key);
    short index = secondaryIndexSearch(secondaryIndex, secondaryIndexNumRec, secondaryKey);
    short RNN = secondaryIndex[index].RNN;
    int prev = -1;
    while (~RNN) {
        if (strcmp(labelIDList[RNN].primaryIndex, key) == 0) {
            if (prev == -1) {
                secondaryIndex[index].RNN = labelIDList[RNN].RNN;
            } else {
                labelIDList[prev].RNN = labelIDList[RNN].RNN;
            }
            labelIDList[RNN].RNN = FREE;
            break;
        }
        prev = RNN;
        RNN = labelIDList[RNN].RNN;
    }
}

void addNewAuthor(Author author) {
    fstream file("authors.txt", ios::in | ios::out | ios::binary);
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char *) &numRec, sizeof(numRec));

    // read from user
    string authorString = author.format(); // format to string
    short len = (short) authorString.size();
    file.seekp(0, ios::end); // default append to end of file if nothing in the avail list
    short RNN = (short) file.tellp();

    // check avail list
    checkAvailList(file, RNN, len);
    if (RNN != file.tellp()) {
        file.seekp(0, ios::end);
    }

    // write to file
    file.write((char *) &len, sizeof(len));
    file.write(authorString.c_str(), len);

    // add the new record to the primary index file
    addToPrimaryIndex("primary_index_authorID.txt", numRec, author.ID, RNN);

    //add the new record to the secondary index file
    addToSecondaryIndex("secondary_index_authorName.txt", "label_id_list.txt", author.name, author.ID);

    // update the number of records
    numRec++;
    file.seekp(sizeof(short), ios::beg);
    file.write((char *) &numRec, sizeof(numRec));
    file.close();
}

void deleteAuthor(string authorID) {
    // open files
    fstream file("authors.txt", ios::in | ios::out | ios::binary);
    fstream primaryIndexFile("primary_index_authorID.txt", ios::in | ios::out | ios::binary);

    // find the author using the primary index
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char *) &numRec, sizeof(numRec));
    PIndex primaryIndex[numRec];
    primaryIndexFile.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndex, numRec, primaryIndexFile);
    int pos = primaryIndexSearch(primaryIndex, numRec, authorID);

    // if not found print not found and return
    if (pos == -1) {
        cout << "Author not found" << endl;
        return;
    }

    // if found then delete the record

    // read the record to know the length of the record
    Author author;
    short len;
    file.seekg(primaryIndex[pos].RNN, ios::beg);
    file.read((char *) &len, sizeof(len));
    char authorString[len + 1];
    file.read(authorString, len);
    authorString[len] = '\0';
    author.parse(authorString);

    // read the header of the avail list
    short AvailList;
    file.seekg(0, ios::beg);
    file.read((char *) &AvailList, sizeof(AvailList));

    // update the header of the avail list
    file.seekp(0, ios::beg);
    string deletedAuthor = formatDeletedRecord(AvailList, len);
    AvailList = primaryIndex[pos].RNN;
    file.write((char *) &AvailList, sizeof(AvailList));

    // update the record with the deleted record
    file.seekp(AvailList, ios::beg);
    short deletedAuthorLen = (short) deletedAuthor.length();
    file.write((char *) &deletedAuthorLen, sizeof(deletedAuthorLen));
    file.write(deletedAuthor.c_str(), len);


    // update the number of records
    file.seekp(sizeof(short), ios::beg);
    numRec--;
    file.write((char *) &numRec, sizeof(numRec));

    // update the primary index in main memory
    for (int i = pos; i < numRec; i++) {
        primaryIndex[i] = primaryIndex[i + 1];
    }
    primaryIndexFile.close();

    // delete from the secondary index
    string authorName = string(author.name);

    // reading the secondary index into main memory
    short secondaryIndexNumRec;
    fstream secondaryIndexFile("secondary_index_authorName.txt", ios::in | ios::out | ios::binary);
    secondaryIndexFile.seekg(0, ios::beg);
    secondaryIndexFile.read((char *) &secondaryIndexNumRec, sizeof(secondaryIndexNumRec));
    SIndex secondaryIndex[secondaryIndexNumRec + 1];
    readSecondaryIndex(secondaryIndex, secondaryIndexNumRec, secondaryIndexFile);
    secondaryIndexFile.close();

    // reading the label id list into main memory
    short labelIDListNumRec;
    fstream labelIDListFile("label_id_list.txt", ios::in | ios::out | ios::binary);
    labelIDListFile.seekg(0, ios::beg);
    labelIDListFile.read((char *) &labelIDListNumRec, sizeof(labelIDListNumRec));
    LabelIDList labelIDList[labelIDListNumRec + 1];
    readLabelIDList(labelIDList, labelIDListNumRec, labelIDListFile);
    labelIDListFile.close();

    removeRecordFromSecondaryIndex(secondaryIndex, labelIDList, secondaryIndexNumRec, author.ID, authorName);

    // update the secondary index file
    updateSecondaryIndex("secondary_index_authorName.txt", secondaryIndex, secondaryIndexNumRec, secondaryIndexFile);

    // update the label id list file
    updateLabelIDList("label_id_list.txt", labelIDList, labelIDListNumRec, labelIDListFile);

    // update the primary index file
    updatePrimaryIndex("primary_index_authorID.txt", primaryIndex, numRec, primaryIndexFile);
}

void updateAuthorName() {
    string authorID;
    cout << "Enter Author ID: " << endl;
    cin >> authorID;

    // open files
    fstream primaryIndexFile("primary_index_authorID.txt", ios::in | ios::out | ios::binary);

    // find the author using the primary index
    fstream file("authors.txt", ios::in | ios::out | ios::binary);
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char *) &numRec, sizeof(numRec));
    file.close();

    PIndex primaryIndex[numRec];
    primaryIndexFile.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndex, numRec, primaryIndexFile);
    primaryIndexFile.close();
    int pos = primaryIndexSearch(primaryIndex, numRec, authorID);

    // if not found print not found and return
    if (pos == -1) {
        cout << "Author not found" << endl;
        return;
    }

    string newAuthorName;
    cout << "Enter new author name: " << endl;
    cin >> newAuthorName;

    // reading the old author
    Author author;

    file.open("authors.txt", ios::in | ios::out | ios::binary);
    file.seekg(primaryIndex[pos].RNN, ios::beg);
    short authorLen;
    file.read((char *) &authorLen, sizeof(authorLen));
    char authorOldRecord[authorLen + 1];
    file.read(authorOldRecord, authorLen);
    file.close();

    authorOldRecord[authorLen] = '\0';
    author.parse(authorOldRecord);

    deleteAuthor(authorID);
    strcpy(author.name, newAuthorName.c_str());
    addNewAuthor(author);
}

void printAuthor(string authorID) {
    fstream file("authors.txt", ios::in | ios::out | ios::binary);
    fstream primaryIndexFile("primary_index_authorID.txt", ios::in | ios::out | ios::binary);
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char *) &numRec, sizeof(numRec));
    PIndex primaryIndex[numRec];
    primaryIndexFile.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndex, numRec, primaryIndexFile);
    int pos = primaryIndexSearch(primaryIndex, numRec, authorID);
    if (pos == -1) {
        cout << "Author not found" << endl;
        return;
    }
    int RNN = primaryIndex[pos].RNN;
    short len;
    file.seekg(RNN, ios::beg);
    file.read((char *) &len, sizeof(len));
    char author[len + 1];
    file.read(author, len);
    author[len] = '\0';
    Author authorObj;
    authorObj.parse(author);
    cout << authorObj << endl;
    file.close();
    primaryIndexFile.close();
}

void addToLabelIDListForBook(string labelFileName, string ISBN, short RNN) {
    fstream labelIDListFile(labelFileName.c_str(), ios::in | ios::out | ios::binary);
    short labelIDListNumRec;
    labelIDListFile.seekg(0, ios::beg);
    labelIDListFile.read((char *) &labelIDListNumRec, sizeof(labelIDListNumRec));
    LabelIDList labelIDList[labelIDListNumRec + 1];
    readLabelIDList(labelIDList, labelIDListNumRec, labelIDListFile);
    labelIDListFile.close();

    short i = 0;
    for (; i <= labelIDListNumRec; i++) {
        if (labelIDList[i].RNN == FREE || i == labelIDListNumRec) {
            strcpy(labelIDList[i].primaryIndex, ISBN.c_str());
            labelIDList[i].RNN = RNN;
            break;
        }
    }
    labelIDListNumRec += (i == labelIDListNumRec);
    updateLabelIDList(labelFileName, labelIDList, labelIDListNumRec, labelIDListFile);
}

void addNewBook(Book book) {
    fstream file("books.txt", ios::in | ios::out | ios::binary);
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char *) &numRec, sizeof(numRec));


    // read from user
    string bookString = book.format(); // format to string
    short len = (short) bookString.size();
    file.seekp(0, ios::end);
    short RNN = (short) file.tellp();

    // check avail list
    checkAvailList(file, RNN, len);
    if (RNN != file.tellp()) {
        file.seekp(0, ios::end);
    }

    // write to file
    file.write((char *) &len, sizeof(len));
    file.write(bookString.c_str(), len);

    // add the new record to the primary index file
    addToPrimaryIndex("primary_index_ISBN.txt", numRec, book.ISBN, RNN);

    // add the new record to the secondary index file
    addToSecondaryIndex("secondary_index_authorID.txt", "label_id_list_books.txt", book.authorID, book.ISBN);

    // add the new record to the label id list
//    addToLabelIDListForBook("label_id_list_books.txt", book.ISBN, RNN);

    // update the number of records
    numRec++;
    file.seekp(sizeof(short), ios::beg);
    file.write((char *) &numRec, sizeof(numRec));
    file.close();
}

void deleteBook(string ISBN) {
    // Open files
    fstream file("books.txt", ios::in | ios::out | ios::binary);
    fstream primaryIndexFile("primary_index_ISBN.txt", ios::in | ios::out | ios::binary);

    // Get the number of books
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char*)&numRec, sizeof(numRec));

    // Read the primary index into main memory
    PIndex primaryIndex[numRec];
    primaryIndexFile.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndex, numRec, primaryIndexFile);
    primaryIndexFile.close();

    // Search for the book using the primary index
    int pos = primaryIndexSearch(primaryIndex, numRec, ISBN);

    // If not found, print a message and return
    if (pos == -1) {
        cout << "Book not found" << endl;
        return;
    }

    // Read the record to know the length of the record
    Book book;
    short len;
    file.seekg(primaryIndex[pos].RNN, ios::beg);
    file.read((char*)&len, sizeof(len));
    char bookString[len + 1];
    file.read(bookString, len);
    bookString[len] = '\0';
    book.parse(bookString);

    // Read the header of the avail list
    short AvailList;
    file.seekg(0, ios::beg);
    file.read((char*)&AvailList, sizeof(AvailList));

    // Update the header of the avail list
    file.seekp(0, ios::beg);
    string deletedBook = formatDeletedRecord(AvailList, len);
    AvailList = primaryIndex[pos].RNN;
    file.write((char*)&AvailList, sizeof(AvailList));

    // Update the record with the deleted record
    file.seekp(AvailList, ios::beg);
    short deletedBookLen = (short)deletedBook.length();
    file.write((char*)&deletedBookLen, sizeof(deletedBookLen));
    file.write(deletedBook.c_str(), len);

    // Update the number of records
    file.seekp(sizeof(short), ios::beg);
    numRec--;
    file.write((char*)&numRec, sizeof(numRec));

    // Update the primary index in main memory
    for (int i = pos; i < numRec; i++) {
        primaryIndex[i] = primaryIndex[i + 1];
    }
    primaryIndexFile.close();

    string authorID = string(book.authorID);

    // reading the secondary index into main memory
    short secondaryIndexNumRec;
    fstream secondaryIndexFile("secondary_index_authorID.txt", ios::in | ios::out | ios::binary);
    secondaryIndexFile.seekg(0, ios::beg);
    secondaryIndexFile.read((char *) &secondaryIndexNumRec, sizeof(secondaryIndexNumRec));
    SIndex secondaryIndex[secondaryIndexNumRec + 1];
    readSecondaryIndex(secondaryIndex, secondaryIndexNumRec, secondaryIndexFile);
    secondaryIndexFile.close();

    // reading the label id list into main memory
    short labelIDListNumRec;
    fstream labelIDListFile("label_id_list_books.txt", ios::in | ios::out | ios::binary);
    labelIDListFile.seekg(0, ios::beg);
    labelIDListFile.read((char *) &labelIDListNumRec, sizeof(labelIDListNumRec));
    LabelIDList labelIDList[labelIDListNumRec + 1];
    readLabelIDList(labelIDList, labelIDListNumRec, labelIDListFile);
    labelIDListFile.close();

    removeRecordFromSecondaryIndex(secondaryIndex, labelIDList, secondaryIndexNumRec, book.ISBN, authorID);

    // update the secondary index file
    updateSecondaryIndex("secondary_index_authorName.txt", secondaryIndex, secondaryIndexNumRec, secondaryIndexFile);

    // update the label id list file
    updateLabelIDList("label_id_list.txt", labelIDList, labelIDListNumRec, labelIDListFile);

    // Update the primary index file
    updatePrimaryIndex("primary_index_ISBN.txt", primaryIndex, numRec, primaryIndexFile);
}


void updateBookTitle(){

    // Get old book title
    string ISBN;
    cout << "Enter Book ISBN: " << endl;
    cin >> ISBN;

    // Get the number of books
    fstream file("books.txt", ios::in | ios::out | ios::binary);
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char*)&numRec, sizeof numRec);
    file.close();

    // Open index file and search for the book
    fstream primaryIndexFile("primary_index_ISBN.txt", ios::in | ios::out | ios::binary);
    PIndex primaryIndexArr[numRec];
    file.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndexArr, numRec, primaryIndexFile);
    int pos = primaryIndexSearch(primaryIndexArr, numRec, ISBN);

    // if not found print not found and return
    if(pos == -1){
        cout << "Book not found" << endl;
        return;
    }
    // Get new book title
    string newBookTitle;
    cout << "Enter new Book title: " << endl;
    cin >> newBookTitle;

    Book book;

    file.open("books.txt", ios::in | ios::out | ios::binary);
    file.seekg(primaryIndexArr[pos].RNN, ios::beg);
    short bookLen;
    file.read((char*) &bookLen, sizeof bookLen);
    char bookOldRecord[bookLen + 1];
    file.read(bookOldRecord, bookLen);
    bookOldRecord[bookLen] = '\0';
    book.parse(bookOldRecord);

    deleteBook(book.ISBN);
    strcpy(book.title, newBookTitle.c_str());
    addNewBook(book);
}


void printBook() {
    string ISBN;
    cout << "Enter ISBN: ";
    cin >> ISBN;

    fstream file("books.txt", ios::in | ios::out | ios::binary);
    fstream primaryIndexFile("primary_index_ISBN.txt", ios::in | ios::out | ios::binary);

    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char *) &numRec, sizeof(numRec));

    PIndex primaryIndex[numRec];
    primaryIndexFile.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndex, numRec, primaryIndexFile);

    int pos = primaryIndexSearch(primaryIndex, numRec, ISBN);

    if (pos == -1) {
        cout << "Book not found" << endl;
        return;
    }

    int RNN = primaryIndex[pos].RNN;
    short len;
    file.seekg(RNN, ios::beg);
    file.read((char *) &len, sizeof(len));
    char bookRecord[len + 1];
    file.read(bookRecord, len);
    bookRecord[len] = '\0';

    Book bookObj;
    bookObj.parse(bookRecord);
    cout << bookObj << endl;

    file.close();
    primaryIndexFile.close();
}

void printBooks(string authorID){
    fstream fileSecondary("secondary_index_authorID.txt", ios::in | ios::binary);
    fstream fileLabelID("label_id_list_books.txt", ios::out | ios::in | ios::binary);
    fstream fileBooks("books.txt", ios::in | ios::binary);
    fstream filePrimary("primary_index_ISBN.txt", ios::in | ios::binary);

    short numRec;
    fileSecondary.seekg(0, ios::beg);
    fileSecondary.read((char*) &numRec, sizeof(numRec));

    SIndex secondaryIndex[numRec];
    readSecondaryIndex(secondaryIndex, numRec, fileSecondary);

    int pos = secondaryIndexSearch(secondaryIndex, numRec, authorID);



    fileLabelID.seekg(0, ios::beg);
    fileLabelID.read((char*) &numRec, sizeof(numRec));

    LabelIDList labelIdList[numRec];
    readLabelIDList(labelIdList, numRec, fileLabelID);

    fileBooks.seekg(sizeof(short), ios::beg);
    fileBooks.read((char*)& numRec, sizeof(numRec));

    PIndex primaryList[numRec];
    readPrimaryIndex(primaryList, numRec, filePrimary);

    pos = secondaryIndex[pos].RNN;

    while(~pos){
        string primaryIndex = labelIdList[pos].primaryIndex;
        int posBook = primaryIndexSearch(primaryList, numRec, primaryIndex);
        fileBooks.seekg(primaryList[posBook].RNN, ios::beg);

        Book book;
        short len;
        fileBooks.read((char*)& len, sizeof(len));
        char bookRecord[len + 1];
        fileBooks.read(bookRecord, len);
        bookRecord[len] = '\0';
        book.parse(bookRecord);

        cout << book << endl;
        pos = labelIdList[pos].RNN;
    }
}

void printAuthorName(string authorID){
    fstream file("authors.txt", ios::in | ios::out | ios::binary);
    fstream primaryIndexFile("primary_index_authorID.txt", ios::in | ios::out | ios::binary);
    file.seekg(sizeof(short), ios::beg);
    short numRec;
    file.read((char *) &numRec, sizeof(numRec));
    PIndex primaryIndex[numRec];
    primaryIndexFile.seekg(0, ios::beg);
    readPrimaryIndex(primaryIndex, numRec, primaryIndexFile);
    int pos = primaryIndexSearch(primaryIndex, numRec, authorID);
    if (pos == -1) {
        cout << "Author not found" << endl;
        return;
    }
    int RNN = primaryIndex[pos].RNN;
    short len;
    file.seekg(RNN, ios::beg);
    file.read((char *) &len, sizeof(len));
    char author[len + 1];
    file.read(author, len);
    author[len] = '\0';
    Author authorObj;
    authorObj.parse(author);
    cout << authorObj.name << endl;
    file.close();
    primaryIndexFile.close();
}
void writeQuery() {
    cout << "1. Select all from Authors where Author ID = xxx" << endl;
    cout << "2. Select all from Books where Author ID = xxx" << endl;
    cout << "3. Select Author Name from Authors where Author ID = xxx" << endl;

    cout << "Enter your choice";
    char choice;
    cin >> choice;

    cout << "Enter ID";
    string id;
    cin >> id;

    if(choice == '1'){
        printAuthor(id);
    }else if (choice == '2'){
        printBooks(id);
    }else if (choice == '3'){
        printAuthorName(id);
    }
}


void exit() {
    exit(0);
}

void printSecondaryIndex() {
    fstream file("secondary_index_authorName.txt", ios::in | ios::binary);
    short numRec;
    file.seekg(0, ios::beg);
    file.read((char *) &numRec, sizeof(numRec));
    SIndex secondaryIndex[numRec];
    readSecondaryIndex(secondaryIndex, numRec, file);
    for (int i = 0; i < numRec; i++) {
        cout << secondaryIndex[i].key << " " << secondaryIndex[i].RNN << endl;
    }
    file.close();
    fstream labelIDListFile("label_id_list.txt", ios::in | ios::binary);
    short labelIDListNumRec;
    labelIDListFile.seekg(0, ios::beg);
    labelIDListFile.read((char *) &labelIDListNumRec, sizeof(labelIDListNumRec));
    LabelIDList labelIDList[labelIDListNumRec];
    readLabelIDList(labelIDList, labelIDListNumRec, labelIDListFile);
    labelIDListFile.close();

    for (int i = 0; i < labelIDListNumRec; i++) {
        cout << labelIDList[i].primaryIndex << " " << labelIDList[i].RNN << endl;
    }
}

void prepareAuthor() {
    fstream f("primary_index_authorID.txt", ios::out | ios::trunc);
    f.close();
    f.open("label_id_list.txt", ios::out | ios::trunc);
    f.close();
    f.open("authors.txt", ios::out | ios::trunc);
    f.close();
    f.open("secondary_index_authorName.txt", ios::out | ios::trunc);
    f.close();

    short AvailList = -1;
    fstream file("authors.txt", ios::in | ios::out | ios::binary);
    file.seekp(0, ios::beg);
    file.write((char *) &AvailList, sizeof(AvailList));
    short size = 0;
    file.write((char *) &size, sizeof(size));
    file.close();
    fstream secondaryIndexFile("secondary_index_authorName.txt", ios::in | ios::out | ios::binary);
    fstream labelIDListFile("label_id_list.txt", ios::in | ios::out | ios::binary);
    secondaryIndexFile.seekp(0, ios::beg);
    labelIDListFile.seekp(0, ios::beg);
    labelIDListFile.write((char *) &size, sizeof(size));
    secondaryIndexFile.write((char *) &size, sizeof(size));
    labelIDListFile.close();
    secondaryIndexFile.close();
}

void prepareBook() {
    // Clear or initialize the necessary files and structures for books
    fstream f("primary_index_ISBN.txt", ios::out | ios::trunc);
    f.close();
    f.open("label_id_list_books.txt", ios::out | ios::trunc);
    f.close();
    f.open("books.txt", ios::out | ios::trunc);
    f.close();
    f.open("secondary_index_authorID.txt", ios::out | ios::trunc);
    f.close();

    short AvailList = -1;
    fstream file("books.txt", ios::in | ios::out | ios::binary);
    file.seekp(0, ios::beg);
    file.write((char *) &AvailList, sizeof(AvailList));
    short size = 0;
    file.write((char *) &size, sizeof(size));
    file.close();

    fstream secondaryIndexFile("secondary_index_authorID.txt", ios::in | ios::out | ios::binary);
    fstream labelIDListFile("label_id_list_books.txt", ios::in | ios::out | ios::binary);
    secondaryIndexFile.seekp(0, ios::beg);
    labelIDListFile.seekp(0, ios::beg);
    labelIDListFile.write((char *) &size, sizeof(size));
    secondaryIndexFile.write((char *) &size, sizeof(size));
    labelIDListFile.close();
    secondaryIndexFile.close();
}

int main() {
    prepareAuthor();
    prepareBook();
    int choice;
    do {
        choice = menu();
        switch (choice) {
            case 1: {
                Author author;
                cin >> author;
                addNewAuthor(author); // done
                break;
            }
            case 2: {
                Book book;
                cin >> book;
                addNewBook(book);
                break;
            }
            case 3:
                updateAuthorName(); // done
                break;
            case 4:
                updateBookTitle();
                break;
            case 5:{
                string ISBN;
                cout << "Enter Book ISBN" << endl;
                cin >> ISBN;
                deleteBook(ISBN);
            }
                break;
            case 6: { // done
                string authorID;
                cout << "Enter Author ID: ";
                cin >> authorID;
                deleteAuthor(authorID);
                break;
            }
            case 7: {
                string authorID;
                cout << "Enter Author ID: " << endl;
                cin >> authorID;
                printAuthor(authorID); // done
                break;
            }
            case 8:
                printBook();
                break;
            case 9:
                writeQuery();
                break;
            case 10:
                exit(); // done
                break;
            case 11:
                printSecondaryIndex();
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    } while (choice != 10);
    return 0;
}