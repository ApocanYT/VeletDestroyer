import pyrebase

from firebasedata import LiveData

config = {
    'apiKey': "AIzaSyCydeZI98pCe67mwq7oPr8g-VdrUGPj5UU",
    'databaseURL': "https://t3veletdestroyer.firebaseio.com",
    'projectId': "t3veletdestroyer",
    'storageBucket': "t3veletdestroyer.appspot.com",
    'messagingSenderId': "595948010085",
    'authDomain': "t3veletdestroyer.firebaseapp.com"
}

app = pyrebase.initialize_app(config)
db = app.database()
storage = app.storage()

test = db.child('test').get()
print(test.val())


def test_handler(sender, value=None):
    if value['test']:
        print("şagalamagalu")
    else:
        print("Никаких тебе шагаламагалу")


live = LiveData(app, '/')
data = live.get_data()
live.signal('/').connect(test_handler)
