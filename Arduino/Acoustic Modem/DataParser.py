import csv
import matplotlib.pyplot as plt
dataVal={}
minSize=20
maxSize=55
window0=[-1]*minSize
window1=[-1]*maxSize
avg0=[]
avg1=[]
output=[]
with open('AcousticData.rtf') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
            #print(f'{row[0]}+{row[1][:-1]}')
            dataVal[row[0]]=row[1][:-1]
            time=int(row[0])
            val=int(row[1][:-1])
            line_count += 1

            window0=[val]+window0[0:-1]
            avg0.append(sum(window0)/minSize)
            # print(window)
            window1=[val]+window1[0:-1]
            avg1.append(sum(window1)/maxSize)

            if (avg1[-1]<850 and avg1[-1]>800):
                output.append(900)
            elif (avg1[-1]<745 and avg1[-1]>700):
                output.append(800)
            else:
                output.append(700)

            

    print(f'Processed {line_count} lines.')
    # print(dataVal)
    plt.plot(list(dataVal.keys())[2000:5000],avg0[2000:5000])
    plt.plot(list(dataVal.keys())[2000:5000],avg1[2000:5000])
    plt.plot(list(dataVal.keys())[2000:5000],output[2000:5000])
    plt.show()