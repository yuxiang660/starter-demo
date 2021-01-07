# Please "allow less secure apps: ON" for your gmail account.
# Refer to https://realpython.com/python-send-email/

import email, smtplib, ssl

from email import encoders
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import os
from subprocess import Popen, PIPE
import subprocess

port = 465  # For SSL
smtp_server = "smtp.gmail.com"
sender_email = "regression.hsv@gmail.com"  # Enter your address
receiver_emails = ["yuxiangw@cadence.com", "yuxiang660@outlook.com"]  # Enter receiver address
password = "admin_tester"

subject = "An email with attachment from Python"
body = "This is an email with attachment sent from Python"

# Create a multipart message and set headers
message = MIMEMultipart()
message["From"] = "yuxiangw@cadence.com"
message["To"] = ", ".join(receiver_emails)
message["Subject"] = subject

# Add body to email
message.attach(MIMEText(body, "plain"))

# Open file in binary mode
filename = "./attach.json"
with open(filename, "rb") as attachment:
    # Add file as application/octet-stream
    # Email client can usually download this automatically as attachment
    part = MIMEBase("application", "octet-stream")
    part.set_payload(attachment.read())

# Encode file in ASCII characters to send by email    
encoders.encode_base64(part)

# Add header as key/value pair to attachment part
part.add_header(
    "Content-Disposition",
    "attachment; filename=summary_results.txt",
)

# Add attachment to message
message.attach(part)

# Log in to server using secure context and send email
context = ssl.create_default_context()
# with smtplib.SMTP_SSL(smtp_server, port, context=context) as server:
#     server.login(sender_email, password)
#     server.sendmail(sender_email, receiver_emails, message.as_string())
print(message.as_string())
# mail="subject:Test\nfrom:yuxiang@cadence.com\nExample Message"
# cmd = f'echo -e "{mail}" | sendmail -t "yuxiangw@cadence.com"'
# process = os.popen(cmd)
# output = process.read()
# process.close()
# print(output)

# p = Popen(["/usr/sbin/sendmail", "-t", "-oi"], stdin=PIPE)
# p.communicate(message.as_bytes())
# print("end")

sendmail_location = "/usr/sbin/sendmail"
print(subprocess.run([sendmail_location, "-t", "-oi"], input=message.as_bytes()))