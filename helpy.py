from kivy.core.text import LabelBase
from kivy.lang import Builder
from kivy.core.window import Window
from kivy.uix.screenmanager import Screen, ScreenManager
from kivymd.app import MDApp
from kivy.uix.boxlayout import BoxLayout

Window.size = (310, 580)


class HomeWindow(Screen):
    pass

class LoginWindow(Screen):
    pass

class SignupWindow(Screen):
    pass

class Happ(MDApp):
    def build(self):
        self.root = BoxLayout(orientation='vertical')

        sm = ScreenManager()
        sm.add_widget(Builder.load_file("hello.kv"))
        sm.add_widget(Builder.load_file("signup.kv"))
        sm.add_widget(Builder.load_file("login.kv"))
        sm.add_widget(Builder.load_file("home.kv"))
        return sm

if __name__ == "__main__":

    LabelBase.register(name="Poppins-Medium", fn_regular="C:\\Users\\renny\\Desktop\\banana\\assets\\fonts\\Poppins\\Poppins-Medium.ttf")
    LabelBase.register(name="Poppins-SemiBold", fn_regular="C:\\Users\\renny\\Desktop\\banana\\assets\\fonts\\Poppins\\Poppins-SemiBold.ttf" )

    Happ().run()
