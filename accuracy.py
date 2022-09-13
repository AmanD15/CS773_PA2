f1 = open("accuracy_s.txt","r")
f2 = open("accuracy_r.txt","r")

count_total = 0
count_correct = 0
sent = f1.readline().strip()
received = f2.readline().strip()
count = min(len(sent),len(received))
count_total += count
	 
for i in range(count):
	if (sent[i]==received[i]):
		count_correct +=1

print("Accuracy is",count_correct/count_total)
f1.close()
f2.close()
