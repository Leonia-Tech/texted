class language():
    def __init__(self, keywords, punctuation, directives, comments, name):
        self.keyw = keywords
        self.punc = punctuation
        self.dir = directives
        self.comments = comments
        self.name = name
        self.string = ['"', "'"]

# Colori

bold = "\x1b[1m"
bold_yellow = "\x1b[1m\x1b[33m"
magenta = "\x1b[35m"
green = "\x1b[32m"
red = "\x1b[31m"
cyan = "\x1b[36m"
reset = "\x1b[0m"
        
