import os
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Sync Perforce Code')
    parser.add_argument(dest='clients', metavar='p4_clients', nargs='+', help='perforce clients')
    args = parser.parse_args()
    p4_clients = args.clients
    for p4_client in p4_clients:
        p4_root = os.popen(f"p4 -c {p4_client} info | grep 'Client root:' | cut -d ' ' -f 3-").read().strip()
        print(p4_root)
        log = os.popen(f"cd {p4_root};ls").read()
        print(log)



