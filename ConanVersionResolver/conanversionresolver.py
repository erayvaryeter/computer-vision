import os

class ConanVersionResolver:

    def __init__(self):
        self.currentDir = os.path.dirname(os.path.abspath(__file__))
        os.chdir(self.currentDir)
        self.tempFilePath = os.path.join(self.currentDir, 'temp.txt')

    def __find_nth_occurance(self, string, substring, n):
        if (n == 1):
            return string.find(substring)
        else:
            return string.find(substring, self.__find_nth_occurance(string, substring, n - 1) + 1)

    def __get_version_count(self, version):
        versionList = version.split('.')
        versionCount = (int(versionList[0]) * 100) + (int(versionList[1]) * 10) + int(versionList[2])
        return versionCount

    def find_suitable_version(self, package_name, minVersion = '0.0.0', maxVersion = '100.0.0'):
        cmd = 'conan search ' + package_name + ' -r conancenter > temp.txt'
        os.system(cmd)
        suitableVersionWithFullString = ''
        suitableVersion = '0.0.0'
        suitableVersionCount = self.__get_version_count(suitableVersion)
        minVersionCount = self.__get_version_count(minVersion)
        maxVersionCount = self.__get_version_count(maxVersion)
        with open(self.tempFilePath) as file:
            for line in file:
                line = line.strip()
                line = line + '/'
                first_occurance = self.__find_nth_occurance(line, '/', 1)
                second_occurance = self.__find_nth_occurance(line, '/', 2)
                if (first_occurance != -1 and second_occurance != -1):
                    version = line[first_occurance+1:second_occurance]
                    versionCount = self.__get_version_count(version)
                    if (versionCount >= minVersionCount and versionCount <= maxVersionCount):
                        if (versionCount >= suitableVersionCount):
                            suitableVersion = version
                            suitableVersionCount = self.__get_version_count(suitableVersion)
                            suitableVersionWithFullString = line[:first_occurance+1] + suitableVersion + line[second_occurance:]
        os.remove('temp.txt')
        if (suitableVersionWithFullString == "0.0.0"):
            print('Any suitable version for the package ' + package_name + ' is not found on gitlab remote')
        return suitableVersionWithFullString

resolver = ConanVersionResolver()
print(resolver.find_suitable_version(package_name='opencv', minVersion='3.1.0', maxVersion='4.3.2'))
print(resolver.find_suitable_version(package_name='zlib'))
print(resolver.find_suitable_version(package_name='boost', maxVersion='1.74.0'))