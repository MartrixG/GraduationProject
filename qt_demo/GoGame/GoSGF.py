class GoSGF(object):
    def __init__(self, f):
        self.fileContext = f.readline()
        self.gameInformation = self.fileContext[2:-1].split(';')
        self.date = 0
        self.blackPlayer = ''
        self.whitePlayer = ''
        self.result = ''
        self.steps = []
        self.initGlobalInformation()
        self.initSteps()

    def srcFile(self):
        return self.fileContext

    def initGlobalInformation(self):
        globalInformation = self.gameInformation[0].split('  ')
        for keys in globalInformation:
            if keys[:2] == 'DT':
                self.date = keys[3:-1]
            elif keys[:2] == 'PB':
                self.blackPlayer = keys[3:-1]
            elif keys[:2] == 'PW':
                self.whitePlayer = keys[3:-1]
            else:
                pos = keys.find('RE')
                if pos != -1:
                    self.result = keys[pos + 3]

    def initSteps(self):
        srcSteps = self.gameInformation[1:]
        for step in srcSteps:
            self.steps.append(Step(step))

    def show(self):
        print('date :\t{:}'.format(self.date))
        print('black player :\t{:}'.format(self.blackPlayer))
        print('white player :\t{:}'.format(self.whitePlayer))
        print('result :\t{:}'.format(self.result))
        for step in self.steps:
            print('{:} {:} {:}'.format(step.player, step.x, step.y))


class Step(object):
    def __init__(self, srcString):
        letterToNumber = {}
        for i in range(19):
            letter = 'a'
            letterToNumber[chr(ord(letter) + i)] = i + 1
        self.player = srcString[0]
        self.x = letterToNumber[srcString[3]]
        self.y = letterToNumber[srcString[2]]