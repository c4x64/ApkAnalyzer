import sys\n\ndef main():\n    print('ApkAnalyzer Python Report Generator')\n    if len(sys.argv) > 1:\n        print(f'Analyzing report: {sys.argv[1]}')\n\nif __name__ == '__main__':\n    main()
