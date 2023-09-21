from kivy.core.text import LabelBase
from kivy.lang import Builder
from kivy.core.window import Window
from kivy.uix.screenmanager import Screen, ScreenManager
from kivymd.app import MDApp
from kivy.uix.boxlayout import BoxLayout
from kivymd.uix.label import MDLabel

Window.size = (310, 580)

class HomeWindow(Screen):
    pass

class LoginWindow(Screen):
    def __init__(self, **kwargs):
        from firebase import firebase
        super(LoginWindow, self).__init__(**kwargs)
        #chama o banco de dados

class SignupWindow(Screen):
    pass

class Happ(MDApp):
    def build(self):
        self.root = BoxLayout(orientation='vertical')

        self.sm = ScreenManager()
        self.sm.add_widget(Builder.load_file("hello.kv"))
        self.sm.add_widget(Builder.load_file("signup.kv"))
        self.sm.add_widget(Builder.load_file("login.kv"))
        self.sm.add_widget(Builder.load_file("home.kv"))
        return self.sm
    
    def login(self, email, password):
        from firebase import firebase

        firebase = firebase.FirebaseApplication('https://helpyweather-b54ab-default-rtdb.firebaseio.com', None)
        #pega o resutado do banco de dados
        
        result = firebase.get('https://helpyweather-b54ab-default-rtdb.firebaseio.com/Users', '')
        
        try:
            for i in result.keys():
                if result[i]['Email'] == email:
                    if result[i]['Password'] == password:
                        self.sm.current = 'home'
        except Exception as e:
            self.error_label = self.ids.error_label
            self.error_label.text = f"Erro de autenticação: {e}"  # Exibir mensagem de erro
            Clock.schedule_once(self.clear_error, 5)

    def registro(self, email, password):
        from firebase import firebase

        firebase = firebase.FirebaseApplication('https://helpyweather-b54ab-default-rtdb.firebaseio.com', None)
        #pega o resutado do banco de dados
        
        result = firebase.get('https://helpyweather-b54ab-default-rtdb.firebaseio.com/Users', '')
        
        try:
            for i in result.keys():
                if result[i]['Email'] == email:
                    if result[i]['Password'] == password:
                        self.sm.current = 'home'
        except Exception as e:
            self.error_label = self.ids.error_label
            self.error_label.text = f"Erro de autenticação: {e}"  # Exibir mensagem de erro
            Clock.schedule_once(self.clear_error, 5)
    
    def clear_error(self, dt):
        error_label = self.ids.error_label
        error_label.text = ''

                

if __name__ == "__main__":
    LabelBase.register(name="Poppins-Medium", fn_regular="C:\\Users\\renny\\Desktop\\banana\\assets\\fonts\\Poppins\\Poppins-Medium.ttf")
    LabelBase.register(name="Poppins-SemiBold", fn_regular="C:\\Users\\renny\\Desktop\\banana\\assets\\fonts\\Poppins\\Poppins-SemiBold.ttf" )
    Happ().run()
