import time
import pymongo
from kivy.core.text import LabelBase
from kivy.lang import Builder
from kivy.core.window import Window
from kivy.uix.screenmanager import Screen, ScreenManager
from kivymd.app import MDApp
from kivymd.uix.behaviors import FakeRectangularElevationBehavior
from kivymd.uix.floatlayout import MDFloatLayout
from kivy.uix.boxlayout import BoxLayout
from kivymd.uix.dialog import MDDialog
from kivymd.uix.button import MDRectangleFlatButton
from kivyauth.google_auth import initialize_google, login_google, logout_google
from kivy.clock import Clock 
import requests
Window.size = (310, 580)
uri = "mongodb+srv://rennyson:rennyson100a@helpy.sllrgwg.mongodb.net/?retryWrites=true&w=majority"

class HomeWindow(Screen):
    pass

class LoginWindow(Screen):
    def __init__(self, **kwargs):
        super(LoginWindow, self).__init__(**kwargs)

class Signup_ok(Screen):
    pass

class Happ(MDApp):
    dialog = None
    def build(self):
        client_id = open("client_id.txt")
        client_secret = open("cliente_secret.txt")
        initialize_google(self.after_login, self.error_listener, client_id.read(), client_secret.read())
        self.root = BoxLayout(orientation='vertical')
        self.sm = ScreenManager()
        self.sm.add_widget(Builder.load_file("assets/templates/home.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/splashscreen.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/hello.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/signup.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/signup_ok.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/login.kv"))
        return self.sm
    
    def on_start(self):
        # Clock.schedule_once(self.change_screen, 10)
        Clock.schedule_interval(self.data_update, 10)
        pass
    
    def change_screen(self, dt):
        self.sm.current = "hello"
    
    def update_general(self, dt):
        cliente = pymongo.MongoClient(uri)
        db = cliente.get_database("seu_banco_de_dados")
    
    def after_login(self):
        pass
    
    def error_listener(self):
        pass

    def login_g(self):
        login_google(self.after_login, self.error_listener)
    
    def data_update(self, dt):
        from firebase import firebase
        try:
            firebase = firebase.FirebaseApplication('https://helpyweather-b54ab-default-rtdb.firebaseio.com', None)
            
            resutumidAr = firebase.get('/sensores/umidadeAr', '')
            resutumidSo = firebase.get('/sensores/umidadeSolo', '')
            resutemp = firebase.get('/sensores/temperatura', '')
            umiAr_str = str(resutumidAr)
            umiSo_str = str(resutumidSo)
            temp_str = str(resutemp)

            print("dados recebidos")
            umidar = self.root.get_screen('home').ids.umidar
            umidsolo = self.root.get_screen('home').ids.umidsolo
            temp = self.root.get_screen('home').ids.temp

            umidar.text = ("umidade do ar: " + umiAr_str)
            umidsolo.text = ("Umidade do Solo: " + umiSo_str)
            temp.text = ("Temperatura Atual: " + temp_str)
            print("definido o texto")

        except Exception as e:
            print("falha nas capturas de dados")

            umidar = self.root.get_screen('home').ids.umidar
            umidsolo = self.root.get_screen('home').ids.umidsolo
            temp = self.root.get_screen('home').ids.temp

            umidar.text = "falha em pegar dados da umidade do Ar"
            umidsolo.text = "falha em pegar dados da umidade do Solo"
            temp.text = "falha em pegar dados de temperatura"

    def login(self, email, password):
        from firebase import firebase
        
        firebase = firebase.FirebaseApplication('https://helpyweather-b54ab-default-rtdb.firebaseio.com', None)
        #pega o resutado do banco de dados
        
        try:
            result = firebase.get('https://helpyweather-b54ab-default-rtdb.firebaseio.com/Users', '')
            for i in result.keys():
                if result[i]['Email'] == email:
                    if result[i]['Password'] == password:
                        self.sm.current = 'home'
        except Exception as e:
            print("Erro de autenticação do login")
            if not self.dialog:
                self.dialog = MDDialog(
                    text = "Seu emaail ou senha estao incorretos!!!",
                    buttons = [
                        MDRectangleFlatButton(
                            text="OK", text_color="#560230")
                    ]
                )
            
            self.dialog.open()

    def registro(self, username, email, password):
        from firebase import firebase

        firebase = firebase.FirebaseApplication('https://helpyweather-b54ab-default-rtdb.firebaseio.com', None)
        
        data = {
            'username': username,
            'Email': email,
            'Password': password
        } 
        #posta el dados
        firebase.post('https://helpyweather-b54ab-default-rtdb.firebaseio.com/Users', data)
        self.sm.current = 'signup_ok'
    
    def clear_error(self, dt):
        self.error_label = self.root.get_screen('login').ids.error_label
        self.error_label.text = ''

if __name__ == "__main__":
    LabelBase.register(name="Poppins-Medium", fn_regular="assets/fonts/Poppins/Poppins-Medium.ttf")
    LabelBase.register(name="Poppins-SemiBold", fn_regular="assets/fonts/Poppins/Poppins-SemiBold.ttf" )
    Happ().run()
