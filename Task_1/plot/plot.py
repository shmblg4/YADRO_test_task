import matplotlib.pyplot as plt
import numpy as np

qpsk = np.loadtxt('../../build/QPSKresults.txt')
qam16 = np.loadtxt('../../build/QAM16results.txt')
qam64 = np.loadtxt('../../build/QAM64results.txt')

plt.figure(figsize=(10, 6))
plt.plot(qpsk[:,0], qpsk[:,1], 'o-', label='QPSK', linewidth=2)
plt.plot(qam16[:,0], qam16[:,1], 's-', label='QAM16', linewidth=2)
plt.plot(qam64[:,0], qam64[:,1], 'd-', label='QAM64', linewidth=2)

plt.title('BER Comparison', fontsize=14)
plt.xlabel('Noise Variance', fontsize=12)
plt.ylabel('BER', fontsize=12)
plt.grid(True)
plt.legend(fontsize=10)

plt.savefig('ber.png', dpi=300, bbox_inches='tight')
plt.show()