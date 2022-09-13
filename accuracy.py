f = open("accuracy.txt","r")

count_total = 0
count_correct = 0
for i in range(4):
	sent = f.readline().strip()
	received = f.readline().strip()
	count = min(len(sent),len(received))
	count_total += count

	 
	for i in range(count):
		if (sent[i]==received[i]):
			count_correct +=1

print("Accuracy is",count_correct/count_total)
f.close()

f = open("accuracy.txt","w")
f.close()
