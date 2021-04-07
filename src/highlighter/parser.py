import re, sys
from string import punctuation
from .language import *

from .C import C_Keywords, C_Punctuation, C_Comment

def SyntaxParser(language, inFile):
    # Boolean init
    allowKeywords = True
    isComment = isPunctuation = isDirective = isDoubleStr = isMultiline = False
    words = re.split(r'([({\t\s!"''%&,:;<=>?^|~!})])', inFile)
    for n in range(len(words)):
        if words[n] == '\n':
            # Reset booleans
            isPunctuation = True
            allowKeywords = True
            if isComment:
                isComment = False
            if isDoubleStr:
                isDoubleStr = False
            if isDirective:
                isDirective = False
            if not isMultiline:
                print(reset, end='')
            print()
        elif language.comments[0] in words[n]:
            print(green + words[n], end='')
            isComment = True
            allowKeywords = False
            isPunctuation = False
        # Handling multiline
        elif "/*" in language.comments and "/*" in words[n]:
            print(green + words[n], end='')
            isMultiline = True
            isPunctuation = False
            allowKeywords = False
        elif "/*" in language.comments and "*/" in words[n] and isMultiline:
            print(green + words[n] + reset, end='')
            isPunctuation = True
            isMultiline = False
        elif isMultiline:
            print(green + words[n] + reset, end='')
        elif language.dir in words[n]:
            print(magenta + words[n] + reset, end='')
            isDirective = True
            allowKeywords = False
            isPunctuation = False
        elif isDirective:
            print(cyan + words[n], end='')
        elif language.string[0] in words[n] and not isComment and not isDirective:
            if not isDoubleStr:
                print(cyan + words[n], end='')
                isDoubleStr = True
                allowKeywords = False
                isPunctuation = False
            elif isDoubleStr:
                print(words[n] + reset, end='')
                isDoubleStr = False
                isPunctuation = True
        elif language.string[1] in words[n]:
            if not isDoubleStr:
                print(cyan + words[n] + reset, end='')
                allowKeywords = False
            else:
                print(words[n], end='')
        elif "/" in words[n] and not isDirective and not isDoubleStr or "*" in words[n] and not isDirective and not isDoubleStr:
            listed = re.split(r'([/*])', words[n])
            for i in range(len(listed)):
                if listed[i] == '/' or listed[i] == '*':
                    print(red + listed[i] + reset, end='')
                else:
                    if listed[i] in language.keyw and allowKeywords:
                        print(bold_yellow + listed[i] + reset, end='')
                    else:
                        print(listed[i], end='')
        elif words[n] in language.punc and isPunctuation and not isDoubleStr:
            print(red + words[n] + reset, end='')
            isPunctuation = True
        elif words[n] in language.keyw and allowKeywords:
            print(bold_yellow + words[n] + reset, end='')
        elif words[n+1] == '(' and not isDoubleStr:
            print(bold + words[n] + reset, end='')
        else:
            print(words[n], end='')
    print()


def main():
    try:
        if len(sys.argv) == 3:
            if sys.argv[2] == "C" or sys.argv[2] == 'c' or sys.argv[2] == 'cpp':
                with open(sys.argv[1], "r") as File:
                    lang = language(C_Keywords, C_Punctuation, "#", C_Comment, "C")
                    SyntaxParser(lang, File.read())
                File.close()
            else:
                print("[Errno 3] Langage not recognized")
        elif len(sys.argv) == 2:
            with open(sys.argv[1], "r") as File:
                print(File.read())
                File.close()
        else:
            print("No arguments passed!")
    except FileNotFoundError as error:
        print(error)

if __name__ == "__main__":
    main()
