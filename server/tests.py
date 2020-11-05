import unittest
import requests


class TestGETMethods(unittest.TestCase):

    def test_get_invalid(self):
        response = requests.get("http://jayashankar.pythonanywhere.com/abcdef/depth")
        # Arrange
        # mocks 
        # Act method.()
        # Assert

        self.assertEqual(response.status_code, 406)

    def test_secret_key_invalid(self):
        response = requests.get("http://jayashankar.pythonanywhere.com/abcde/depth")
        self.assertEqual(response.status_code, 401)

    def test_get_switch_on(self):
        response = requests.get("http://jayashankar.pythonanywhere.com/abcdef/depth?d=2")
        response_switch = response.json()["switch"]
        result_switch =1
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response_switch, result_switch)
        
    def test_get_switch_off(self):
        response = requests.get("http://jayashankar.pythonanywhere.com/abcdef/depth?d=9")
        response_switch = response.json()["switch"]
        result_switch =0
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response_switch, result_switch)


class TestPOSTMethods(unittest.TestCase):

    def test_post_invalid(self):
        response = requests.post("http://jayashankar.pythonanywhere.com/abcdef/depth")
        self.assertEqual(response.status_code, 406)

    def test_post_switch_on(self):
        body = {'depth': 2}
        response = requests.post("http://jayashankar.pythonanywhere.com/abcdef/depth",json=body)
        response_switch= response.json()["switch"]
        result_switch =1
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response_switch, result_switch)
        
    def test_post_switch_off(self):
        body = {'depth': 9}
        response = requests.post("http://jayashankar.pythonanywhere.com/abcdef/depth",json=body)
        response_switch = response.json()["switch"]
        result_switch = 0
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response_switch, result_switch)

if __name__ == '__main__':
    unittest.main()