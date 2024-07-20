#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

string FileName = "client.csv";
string delim = ",";
struct sClient
{
  string AccountNumber;
  string PinCode;
  string Name;
  string Phone;
  double AccountBalance;
  bool MarkForDelete;
};
vector<sClient> clients;
void startApp();
void showMainScreen();
;
enum Operation
{
  ShowAll = 1,
  Add = 2,
  Delete = 3,
  Update = 4,
  Find = 5,
  Transactions = 6,
  Exit = 7,
};
enum Transactions
{
  Deposit = 1,
  withdraw = 2,
  TotalBalances = 3,
  MainMenu = 4,
};
string readString(string message)
{
  string theString;
  cout << "\n"
       << message;
  getline(cin >> ws, theString);
  return theString;
}
sClient ReadNewClient()
{
  sClient client;
  string delimter = "#||#";
  cout << "Please enter client data:\n";
  client.AccountNumber = readString("enter account number: ");
  client.PinCode = readString("enter pinCode: ");
  client.Name = readString("enter Name: ");
  client.Phone = readString("enter phone: ");
  cout << "\nenter account Balance: ";
  cin >> client.AccountBalance;
  return client;
}
void goToMainScreen()
{
  cout << "press any key to back to main menu..." << endl;
  system("pause>0");
  showMainScreen();
}
void transactionsScreen();
void goToTransactionsScreen()
{
  cout << "press any key to back to Transactions screen..." << endl;
  system("pause>0");
  transactionsScreen();
}
string clientToLine(sClient client, string delimter)
{
  return (
      client.AccountNumber + delimter +
      client.PinCode + delimter +
      client.Name + delimter +
      client.Phone + delimter +
      to_string(client.AccountBalance));
}
vector<string> splitString(string s, string Delimiter)
{
  int position;
  string word;
  vector<string> words;
  while ((position = s.find(Delimiter)) != std::string::npos)
  {
    word = s.substr(0, position);
    if (word != "")
    {
      words.push_back(word);
    }
    s.erase(0, position + Delimiter.length());
  }
  if (s != "")
  {
    words.push_back(s);
  }
  return words;
}
sClient convertLineToRecord(string clientLine, string delimter)
{
  sClient client;
  vector<string> records = splitString(clientLine, delimter);
  client.AccountNumber = records[0];
  client.PinCode = records[1];
  client.Name = records[2];
  client.Phone = records[3];
  client.AccountBalance = stod(records[4]);
  return client;
}
void printClientRecord(sClient client)
{
  cout << "<===:The client record is:===> \n\n";
  cout << "    Account number   : " << client.AccountNumber << endl;
  cout << "    Account pinCode  : " << client.PinCode << endl;
  cout << "    Client  name     : " << client.Name << endl;
  cout << "    Account phone    : " << client.Phone << endl;
  cout << "    Account balance  : " << client.AccountBalance << "\n"
       << endl;
}
void AddDataLineToFile(string fileName, string clientLine)
{
  fstream MyFile;
  MyFile.open(FileName, ios::out | ios::app);
  if (MyFile.is_open())
  {
    MyFile << clientLine << endl;
    MyFile.close();
  }
}
void AddClients()
{
  char again = 'y';
  while (tolower(again) == 'y')
  {
    system("cls");
    sClient client = ReadNewClient();
    AddDataLineToFile(FileName, clientToLine(client, ","));
    printClientRecord(client);
    cout << "To add more clients enter y" << endl;
    cin >> again;
  }
}
vector<sClient> loadClientsDataFromFile(string FileName)
{
  fstream MyFile;
  vector<sClient> clients;
  MyFile.open(FileName, ios::in); // read only
  if (MyFile.is_open())
  {
    string line;
    sClient client;
    while (getline(MyFile, line))
    {
      client = convertLineToRecord(line, delim);
      clients.push_back(client);
      // printClientRecord(client);
    }
    MyFile.close();
  }
  return clients;
}
void printClientsHeader(int count)
{
  cout << "                     client list (" << count << ") client(s)                   " << endl;
  cout << "===========================================================================" << endl;
  cout << "| Account Number  | Pin code  | Client Name       | Phone       | Balance  ";
  cout << "\n===========================================================================" << endl;
}
void printClientsBalanceHeader(int count)
{
  cout << "                     client list (" << count << ") client(s)                   " << endl;
  cout << "===========================================================" << endl;
  cout << "| Account Number  | Client Name             | Balance  ";
  cout << "\n===========================================================" << endl;
}
void printClient(sClient client)
{
  cout << "| " << left << setw(16) << client.AccountNumber;
  cout << "| " << left << setw(10) << client.PinCode;
  cout << "| " << left << setw(18) << client.Name;
  cout << "| " << left << setw(12) << client.Phone;
  cout << "| " << left << setw(9) << client.AccountBalance << "\n";
  cout << "---------------------------------------------------------------------------" << endl;
}
void printClientBalance(sClient client)
{
  cout << "| " << left << setw(16) << client.AccountNumber;
  cout << "| " << left << setw(24) << client.Name;
  cout << "| " << left << setw(9) << client.AccountBalance << "\n";
  cout << "----------------------------------------------------------" << endl;
}
void showClientsData(vector<sClient> clients, bool isBalanceData = false)
{
  int totalBalances = 0;
  isBalanceData ? printClientsBalanceHeader(clients.size()) : printClientsHeader(clients.size());
  for (sClient &client : clients)
  {
    totalBalances += client.AccountBalance;
    isBalanceData ? printClientBalance(client) : printClient(client);
  }
  if (isBalanceData)
    cout << "                     Total Balalances : " << totalBalances << endl;
}
bool findClientByAccountNumber(vector<sClient> clients, string AccountNumber, sClient &client)
{
  for (sClient &c : clients)
  {
    if (c.AccountNumber == AccountNumber)
    {
      client = c;
      return true;
    }
  }
  return false;
}
bool MarkClientForDelete(vector<sClient> &clients, string AccountNumber)
{
  for (sClient &c : clients)
  {
    if (c.AccountNumber == AccountNumber)
    {
      c.MarkForDelete = true;
      return true;
    }
  }
  return false;
}
string ReadAccountNumber()
{
  return readString("enter AccountNumber to find client?\n");
}
void SaveClientsDataToFile(string fileName, vector<sClient> &clients)
{
  fstream MyFile;
  MyFile.open(FileName, ios::out);
  if (MyFile.is_open())
  {
    for (sClient &client : clients)
    {
      if (!client.MarkForDelete)
      {
        MyFile << clientToLine(client, ",") << endl;
      }
    }

    MyFile.close();
  }
}
bool DeleteClientByAccountNumber(vector<sClient> &clients, string AccountNumber)
{
  sClient client;
  char Answer = 'n';
  if (findClientByAccountNumber(clients, AccountNumber, client))
  {
    printClientRecord(client);
    cout << "\n\nAre you sure you want delete this client? [y/n]";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
      MarkClientForDelete(clients, AccountNumber);
      SaveClientsDataToFile(FileName, clients);
      // refresh data
      clients = loadClientsDataFromFile(FileName);
      cout << "\n\nClient deleted successfully";
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    cout << "The client with account number (" << AccountNumber << ") not found.";
  }
  return false;
}
bool UpdateClientByAccountNumber(vector<sClient> &clients, string AccountNumber)
{
  sClient client;
  char Answer = 'n';
  if (findClientByAccountNumber(clients, AccountNumber, client))
  {
    printClientRecord(client);
    cout << "\n\nAre you sure you want update this client? [y/n]";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
      for (sClient &client : clients)
      {
        if (client.AccountNumber == AccountNumber)
        {
          cout << "Please enter client updated data:\n";
          client.PinCode = readString("enter pinCode: ");
          client.Name = readString("enter Name: ");
          client.Phone = readString("enter phone: ");
          cout << "\nenter account Balance: ";
          cin >> client.AccountBalance;
          break;
        }
      }
      SaveClientsDataToFile(FileName, clients);
      cout << "\n\nClient update successfully";
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    cout << "The client with account number (" << AccountNumber << ") not found.";
  }
  return false;
}
bool DepositOrWithdrawByAccountNumber(vector<sClient> &clients, string AccountNumber, bool isDeposit = true)
{
  sClient client;
  char Answer = 'n';
  if (findClientByAccountNumber(clients, AccountNumber, client))
  {
    printClientRecord(client);
    int amount = 0;
    cout << (isDeposit ? "\n\nPlease enter deposit amount ?" : "\n\nPlease enter withdraw amount ?");
    cin >> amount;
    while (amount > client.AccountBalance && !isDeposit)
    {
      cout << "\nAmount exceeds the balance you can withdraw up to : " << (client.AccountBalance) << endl;
      cout << "\n\nPlease enter withdraw amount ?";
      cin >> amount;
    }
    cout << "\n\nAre you sure you want perform this this transaction? [y/n]";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
      for (sClient &client : clients)
      {
        if (client.AccountNumber == AccountNumber)
        {
          if (isDeposit)
          {
            client.AccountBalance += amount;
            break;
          }
          else
          {
            client.AccountBalance -= amount;
          }
        }
      }
      SaveClientsDataToFile(FileName, clients);
      cout << "\n\nClient update successfully";
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    cout << "The client with account number (" << AccountNumber << ") not found.";
  }
  return false;
}
short readOption(string fromTo)
{
  short op = 0;
  cout << "Choose what do you want to do? [" + fromTo + "]:";
  cin >> op;
  return op;
}
void findClientScreen()
{
  sClient client;
  cout << "======================================" << endl;
  cout << "                Find screen           " << endl;
  cout << "======================================" << endl;
  string AccountNumber = ReadAccountNumber();
  if (findClientByAccountNumber(clients, AccountNumber, client))
  {
    printClientRecord(client);
  }
  else
  {
    cout << "The client with account number (" << AccountNumber << ") not found.";
  }
}
void deleteClientScreen()
{
  cout << "======================================" << endl;
  cout << "              Delete screen           " << endl;
  cout << "======================================" << endl;
  DeleteClientByAccountNumber(clients, ReadAccountNumber());
}
void updateClientScreen()
{
  cout << "======================================" << endl;
  cout << "              Update screen           " << endl;
  cout << "======================================" << endl;
  UpdateClientByAccountNumber(clients, ReadAccountNumber());
}
void depositScreen()
{
  cout << "======================================" << endl;
  cout << "             Deposit screen           " << endl;
  cout << "======================================" << endl;
  DepositOrWithdrawByAccountNumber(clients, ReadAccountNumber());
};
void withdrawScreen()
{
  cout << "======================================" << endl;
  cout << "            Withdraw screen           " << endl;
  cout << "======================================" << endl;
  DepositOrWithdrawByAccountNumber(clients, ReadAccountNumber(), false);
};
void totalBalancesScreen()
{
  showClientsData(clients, true);
};
void manageTransactionsScreens(vector<sClient> &clients, short operationKey)
{
  switch (operationKey)
  {
  case Transactions::Deposit:
    system("cls");
    depositScreen();
    goToTransactionsScreen();
    break;
  case Transactions::withdraw:
    system("cls");
    withdrawScreen();
    goToTransactionsScreen();
    break;
  case Transactions::TotalBalances:
    system("cls");
    totalBalancesScreen();
    goToTransactionsScreen();
    break;
  default:
    showMainScreen();
    break;
  }
}
void transactionsScreen()
{
  system("cls");
  cout << "======================================" << endl;
  cout << "           Transactions screen        " << endl;
  cout << "======================================" << endl;
  cout << "\t [1] Deposit. " << endl;
  cout << "\t [2] withdraw. " << endl;
  cout << "\t [3] Total Balances.  " << endl;
  cout << "\t [4] Main menu. " << endl;
  cout << "======================================" << endl;
  manageTransactionsScreens(clients, readOption("1 to 4"));
}
void manageOperationScreens(vector<sClient> &clients, short operationKey)
{
  switch (operationKey)
  {
  case Operation::ShowAll:
    system("cls");
    showClientsData(clients);
    goToMainScreen();
    break;
  case Operation::Add:
    system("cls");
    AddClients();
    clients = loadClientsDataFromFile(FileName);
    goToMainScreen();
    break;
  case Operation::Find:
    system("cls");
    findClientScreen();
    goToMainScreen();
    break;
  case Operation::Update:
    system("cls");
    updateClientScreen();
    goToMainScreen();
    break;
  case Operation::Delete:
    system("cls");
    deleteClientScreen();
    goToMainScreen();
    break;
  case Operation::Transactions:
    system("cls");
    transactionsScreen();
    goToMainScreen();
    break;
  default:
    break;
  }
}

void showMainScreen()
{
  system("cls");
  cout << "======================================" << endl;
  cout << "           Main menu screen           " << endl;
  cout << "======================================" << endl;
  cout << "\t [1] Show client list. " << endl;
  cout << "\t [2] Add new client. " << endl;
  cout << "\t [3] Delete client.  " << endl;
  cout << "\t [4] Update client info. " << endl;
  cout << "\t [5] Find client. " << endl;
  cout << "\t [6] Transactions. " << endl;
  cout << "\t [7] Exit. " << endl;
  cout << "======================================" << endl;
  manageOperationScreens(clients, readOption("1 to 7"));
}

void startApp()
{
  showMainScreen();
}
int main()
{
  clients = loadClientsDataFromFile(FileName);
  startApp();
  return 0;
}
