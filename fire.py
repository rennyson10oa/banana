from firebase import firebase

#inicia o banco de dados
firebase = firebase.FirebaseApplication('https://helpyweather-b54ab-default-rtdb.firebaseio.com', None)

#data = {
#    'Email': 'rennysonlimadiasrennyson@gmail.com',
#    'Password': 'rennyson100a'
#} 
#posta el dados
#firebase.post('https://helpyweather-b54ab-default-rtdb.firebaseio.com/Users', data)

# Get Data
result = firebase.get('https://helpyweather-b54ab-default-rtdb.firebaseio.com/Users', '')
# Get Specific column like email or password

for i in result.keys():
    print(result[i]['Password'])