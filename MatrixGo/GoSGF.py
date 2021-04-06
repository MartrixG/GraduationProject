from MatrixGo.GoGame import Point


class GoSGF(object):
    def __init__(self, f):
        self.len = None
        self.fileContext = f
        self.gameInformation = self.fileContext[2:-1].split(';')
        self.RU = None
        self.RE = None
        self.KM = None
        self.HA = None
        self.nowStep = 0
        self.steps = []
        self.initGlobalInformation()
        self.initSteps()

    def srcFile(self):
        return self.fileContext

    def initGlobalInformation(self):
        globalInformation = self.gameInformation[0]
        RUPos = globalInformation.find('RU')
        REPos = globalInformation.find('RE')
        KMPos = globalInformation.find('KM')
        HAPos = globalInformation.find('HA')
        self.RU = globalInformation[RUPos + 3:RUPos + 4] if RUPos != -1 else None
        self.RE = globalInformation[REPos + 3:REPos + 4] if REPos != -1 else None
        self.KM = float(globalInformation[KMPos + 3:KMPos + 4]) if KMPos != -1 else None
        self.HA = int(globalInformation[HAPos + 3:HAPos + 4])

    def initSteps(self):
        letterToNumber = {}
        for i in range(19):
            letter = 'a'
            letterToNumber[chr(ord(letter) + i)] = i
        if self.HA != 0:
            handCap = self.gameInformation[1]
            for i in range(self.HA):
                capStr = 'B['
                tmpStep = Step()
                tmpStep.initFromSGF(capStr + handCap[3 + 4 * i] + handCap[4 + 4 * i], letterToNumber)
                self.steps.append(tmpStep)
        bias = 1 + (0 if self.HA == 0 else 1)
        for i in range(len(self.gameInformation) - bias):
            step = self.gameInformation[i + bias]
            tmpStep = Step()
            tmpStep.initFromSGF(step, letterToNumber)
            self.steps.append(tmpStep)
        self.len = len(self.steps)

    def show(self):
        print('RU :\t{:}'.format(self.RU))
        print('KM :\t{:}'.format(self.KM))
        print('result :\t{:}'.format(self.RE))
        for step in self.steps:
            print('{:} {:} {:}'.format(step.player, step.x, step.y))

    def haveNextStep(self):
        return self.nowStep < self.len

    def getNextStep(self):
        self.nowStep += 1
        return self.steps[self.nowStep - 1]


class Step(object):
    def __init__(self):
        self.player = None
        self.x = None
        self.y = None

    def check(self):
        assert self.player is not None
        assert self.x is not None
        assert self.y is not None
        assert self.player == 'B' or self.player == 'W'
        assert 0 <= self.x <= 18 and 0 <= self.y <= 18

    def initFromSGF(self, srcString, letterToNumber):
        self.player = srcString[0]
        x = srcString[3]
        y = srcString[2]
        if 'a' <= x <= 'z' and 'a' <= y <= 'z':
            self.x = letterToNumber[srcString[3]]
            self.y = letterToNumber[srcString[2]]
        # self.check()

    def initFromArgs(self, player, point):
        if not isinstance(point, Point):
            return False
        self.player = player
        self.x, self.y = point.x, point.y

    def initFromCommandLine(self, x, y, player):
        self.player = player
        self.x = x
        self.y = y
        # self.check()

    def __str__(self):
        return str(self.x) + ' ' + str(self.y)

    def __repr__(self):
        return self.__str__()
