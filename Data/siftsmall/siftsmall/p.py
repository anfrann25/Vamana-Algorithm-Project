import struct

def read_ivec_file(file_path):
    vectors = []
    with open(file_path, 'rb') as f:
        while True:
            # Διαβάζουμε το μήκος του διανύσματος (4 bytes ακέραιος)
            vector_size_data = f.read(4)
            if not vector_size_data:
                break  # Τέλος αρχείου

#Μετατρέπουμε το μήκος του διανύσματος σε ακέραιο
            vector_size = struct.unpack('i', vector_size_data)[0]

            # Διαβάζουμε τα υπόλοιπα στοιχεία του διανύσματος
            vector_data = f.read(4 * vector_size)
            if len(vector_data) != 4 * vector_size:
                raise ValueError("Το αρχείο δεν έχει το αναμενόμενο μέγεθος δεδομένων.")

#Μετατρέπουμε τα δεδομένα σε λίστα από ακέραιους
            vector = struct.unpack(f'{vector_size}i', vector_data)
            vectors.append(vector)

    return vectors

#Παράδειγμα χρήσης
file_path = 'siftsmall_groundtruth.ivecs'  # Δώσε το μονοπάτι του αρχείου εδώ
vectors = read_ivec_file(file_path)

#Εκτυπώνουμε τα διαβασμένα διανύσματα
for i, vec in enumerate(vectors):
    print(f"Vector {i}: {vec}")