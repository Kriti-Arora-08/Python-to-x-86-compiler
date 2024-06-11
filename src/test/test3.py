class BankAccount:
    def __init__(self, balance: int):
        self.balance = balance
    def deposit(self, amount: int):
        self.balance += amount
    def withdraw(self, amount: int):
        if amount <= self.balance:
            self.balance -= amount
        else:
            print("Insufficient funds")

class Transaction:
    def __init__(self, sender: str, receiver: str, amount: int):
        self.sender = sender
        self.receiver = receiver
        self.amount = amount
    
    def execute(self):
        if self.sender in bank.accounts and self.receiver in bank.accounts:
            if bank.accounts[self.sender].balance >= self.amount:
                bank.accounts[self.sender].withdraw(self.amount)
                bank.accounts[self.receiver].deposit(self.amount)
                print("Transaction successful")
            else:
                print("Transaction failed: Insufficient funds")
        else:
            print("Invalid account numbers")

class Bank:
    def __init__(self):
        self.accounts = []
<<<<<<< HEAD
    def add_account(self, account_number: str, initial_balance: int):
        self.accounts[account_number] = BankAccount(initial_balance)
=======
    
    def add_account(self, account_number: str, initial_balance: int):
        self.accounts[account_number] = BankAccount(initial_balance)
    
>>>>>>> origin/main
    def transfer(self, sender_account_number: str, receiver_account_number: str, amount: int):
        transaction = Transaction(sender_account_number, receiver_account_number, amount)
        transaction.execute()

bank = Bank()
bank.add_account("123456", 1000)
bank.add_account("789012", 500)

print("Before transaction:")
print("Account 123456 balance:") 
print(bank.accounts["123456"].balance)
print("Account 789012 balance:")
print(bank.accounts["789012"].balance)

bank.transfer("123456", "789012", 300)

print("After transaction:")
print("Account 123456 balance:")
print(bank.accounts["123456"].balance)
print("Account 789012 balance:")
print(bank.accounts["789012"].balance)
