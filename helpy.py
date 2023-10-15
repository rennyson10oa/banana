from kivy.core.text import LabelBase
from kivy.lang import Builder
from kivy.core.window import Window
from kivy.uix.screenmanager import Screen, ScreenManager
from kivymd.app import MDApp
from kivymd.uix.behaviors import FakeRectangularElevationBehavior
from kivymd.uix.floatlayout import MDFloatLayout
from kivy.uix.boxlayout import BoxLayout
from kivyauth.google_auth import initialize_google, login_google, logout_google
from kivy.clock import Clock 

Window.size = (310, 580)

class HomeWindow(Screen):
    pass

class LoginWindow(Screen):
    def __init__(self, **kwargs):
        super(LoginWindow, self).__init__(**kwargs)

class Signup_ok(Screen):
    pass

class Happ(MDApp):
    def build(self):
        client_id = open("client_id.txt")
        client_secret = open("cliente_secret.txt")
        initialize_google(self.after_login, self.error_listener, client_id.read(), client_secret.read())
        self.root = BoxLayout(orientation='vertical')
        self.sm = ScreenManager()
        self.sm.add_widget(Builder.load_file("assets/templates/hello.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/signup.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/signup_ok.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/login.kv"))
        self.sm.add_widget(Builder.load_file("assets/templates/home.kv"))
        return self.sm
    
    def after_login(self):
        pass
    
    def error_listener(self):
        pass

    def login_g(self):
        login_google(self.after_login, self.error_listener)
        
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
            self.error_label = self.ids.error_label
            self.error_label.text = f"Erro de autenticação: {e}"  # Exibir mensagem de erro
            Clock.schedule_once(self.clear_error, 5)

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
