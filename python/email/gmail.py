# Please "allow less secure apps: ON" for your gmail account.
# Refer to https://realpython.com/python-send-email/

import smtplib, ssl
from email.mime.text import MIMEText

port = 465  # For SSL
smtp_server = "smtp.gmail.com"
sender_email = "regression.hsv@gmail.com"  # Enter your address
receiver_emails = ["yuxiangw@cadence.com", "yuxiang660@outlook.com"]  # Enter receiver address
password = "admin_tester"
msg = MIMEText("This message is sent from Python.")
msg['Subject'] = "subject line"
msg['From'] = sender_email
msg['To'] = ", ".join(receiver_emails)

context = ssl.create_default_context()
with smtplib.SMTP_SSL(smtp_server, port, context=context) as server:
    server.login(sender_email, password)
    server.sendmail(sender_email, receiver_emails, msg.as_string())
