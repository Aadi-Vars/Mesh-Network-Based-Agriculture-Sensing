from flask import Flask, request
import requests

app = Flask(__name__)

GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbzo7fIXD77pvr3BR-zbSJvrskjFvrt35SUJX1_buR-2FSs36G4aStMcIq3wce2CcvPSeQ/exec"

@app.route('/', methods=['GET'])
def relay():
    params = request.args.to_dict()

    # Check if it's a batch request
    if "batch" in params:
        batch_data = params["batch"]
        entries = batch_data.split("|")  # Each entry like '23,45,50,n6'
        responses = []
        for entry in entries:
            try:
                s1, s2, s3, node = entry.split(",")
                forward_params = {
                    "hum": s1,
                    "humi": s2,
                    "humii": s3,
                    "name": node
                }
                res = requests.get(GOOGLE_SCRIPT_URL, params=forward_params)
                responses.append(res.text)
            except Exception as e:
                responses.append(f"Error in batch entry '{entry}': {e}")
        return "\n".join(responses)

    # Else, forward as usual
    try:
        res = requests.get(GOOGLE_SCRIPT_URL, params=params)
        return res.text
    except Exception as e:
        return f"Error: {e}"
