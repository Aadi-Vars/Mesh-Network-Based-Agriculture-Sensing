from flask import Flask, request
import requests

app = Flask(__name__)

@app.route('/', methods=['GET'])
def relay():
    google_script_url = "https://script.google.com/macros/s/AKfycbxZxfqcJrEVkGLeJO0kdpPls1WFvyTCGvrTq4kcZ7KKBL4dQAGplT4CiGJJ8fjwld6u/exec"
    params = request.args.to_dict()
    try:
        res = requests.get(google_script_url, params=params)
        return res.text
    except Exception as e:
        return f"Error: {e}"
