from flask import Flask, request, jsonify
import requests

app = Flask(__name__)

@app.route('/receive', methods=['POST'])
def receive():
    if request.method == 'POST':
        data = request.get_json()
        print("Received data:", data)
        return jsonify({"message": "Data received successfully"}), 200

@app.route('/send', methods=['POST'])
def send():
    if request.method == 'POST':
        data = request.get_json()
        target_url = data.get('url')
        payload = data.get('payload')

        if not target_url or not payload:
            return jsonify({"error": "Invalid request format"}), 400

        try:
            response = requests.post(target_url, json=payload)
            response_data = response.json()
            return jsonify(response_data), response.status_code
        except Exception as e:
            return jsonify({"error": str(e)}), 500

    
if __name__ == "__main__":
    # from waitress import serve
    # serve(app, host="0.0.0.0", port=8080)
    app.run(host='0.0.0.0',debug=True)
