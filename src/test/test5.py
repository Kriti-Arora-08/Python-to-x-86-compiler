class Email:
    def __init__(self, sender: str, receiver: str, subject: str, message: str):
        self.sender = sender
        self.receiver = receiver
        self.subject = subject
        self.message = message
    def display_info(self) -> None:
        print("From:", self.sender)
        print("To:", self.receiver)
        print("Subject:", self.subject)
        print("Message:", self.message)

class Inbox:
    def __init__(self):
        self.emails = []
    def receive_email(self, email: Email) -> None:
        self.emails.append(email)
    def display_inbox(self) -> None:
        print("Inbox:")
        for email in self.emails:
            email.display_info()
    def filter_by_sender(self, sender: str) -> None:
        filtered_emails = [email for email in self.emails if email.sender == sender]
        print("Emails from:")
        print(sender)
        for email in filtered_emails:
            email.display_info()
    def filter_by_receiver(self, receiver: str) -> None:
        filtered_emails = [email for email in self.emails if email.receiver == receiver]
        print("Emails to")
        print(receiver)
        for email in filtered_emails:
            email.display_info()

inbox = Inbox()

email1 = Email("john@example.com", "alice@example.com", "Meeting Agenda", "Hello Alice, here's the agenda for tomorrow's meeting.")
email2 = Email("bob@example.com", "alice@example.com", "Project Update", "Hi Alice, please find attached the latest project update.")
email3 = Email("alice@example.com", "john@example.com", "Re: Meeting Agenda", "Hi John, thanks for sending the agenda. Looks good.")

inbox.receive_email(email1)
inbox.receive_email(email2)
inbox.receive_email(email3)

inbox.display_inbox()

inbox.filter_by_sender("alice@example.com")
inbox.filter_by_receiver("alice@example.com")
