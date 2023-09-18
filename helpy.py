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

class RegisterWindow(Screen):
    pass

class Happ(MDApp):
    def build(self):
        self.root = BoxLayout(orientation='vertical')


        sm = ScreenManager()
        sm.add_widget(HomeWindow(name='home'))
        sm.add_widget(LoginWindow(name='login'))
        sm.add_widget(RegisterWindow(name='register'))
        return sm

if __name__ == '__main__':
    Happ().run()
