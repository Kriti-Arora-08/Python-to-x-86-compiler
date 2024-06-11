class Book:
    def __init__(self, title: str, author: str, copies: int):
        self.title = title
        self.author = author
        self.copies = copies
    def display_info(self):
        print("Title:")
        print(self.title)
        print("Author:")
        print(self.author)
        print("Copies available:")
        print(self.copies)

class Library:
    def __init__(self):
        self.books = []
    def add_book(self, book: Book):
        self.books.append(book)
    def display_books(self):
        print("Library Books:")
        for book in self.books:
            print(book.title)
            print(book.author)
    def find_book(self, title: str) -> Book:
        for book in self.books:
            if book.title == title:
                return book
        return None
    def lend_book(self, title: str):
        book = self.find_book(title)
        if book:
            if book.copies > 0:
                book.copies -= 1
                print("Book has been lent.")
            else:
                print("Sorry, no copies available.")
        else:
            print("Book not found in library.")
    def return_book(self, title: str):
        book = self.find_book(title)
        if book:
            book.copies += 1
            print("Book has been returned.")
        else:
            print("Book not found in library.")

library = Library()

book1 = Book("The Great Gatsby", "F. Scott Fitzgerald", 5)
book2 = Book("To Kill a Mockingbird", "Harper Lee", 3)
book3 = Book("1984", "George Orwell", 2)

library.add_book(book1)
library.add_book(book2)
library.add_book(book3)

library.display_books()

library.lend_book("The Great Gatsby")
library.lend_book("1984")
library.lend_book("To Kill a Mockingbird")

library.display_books()

library.return_book("1984")
library.return_book("The Great Gatsby")

library.display_books()
