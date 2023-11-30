from firebase import firebase

firebase = firebase.FirebaseApplication('https://helpyweather-b54ab-default-rtdb.firebaseio.com', None)
            
result = firebase.get('/sensores/temperatura', '')
print(result)