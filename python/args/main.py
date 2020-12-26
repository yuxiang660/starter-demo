import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='P4 TO GIT Tool')
    parser.add_argument('action', choices=['clone', 'sync'], help='choose action (clone or sync) for p4_to_git tool')
    parser.add_argument('config_file', help='configuration file for p4_to_git tool')
    parser.add_argument('-f', '--force', dest='sync_force', action='store_true', help='sync the reop forcely')
    args = parser.parse_args()

    if args.action == 'clone':
        print('--clone---')
    else:
        print('---sync---')
    print(args.action)
    print(args.config_file)
    print(args.sync_force)
