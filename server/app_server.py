from flask import Flask, request, jsonify
import json

app = Flask(__name)

vehicle_data = {
    "latitude": 0.0,
    "longitude": 0.0,
    "passenger_count": 0
}

@app.route('/update_vehicle_data', methods=['POST'])
def update_vehicle_data():
    data = request.get_json()

    latitude = data.get('latitude')
    longitude = data.get('longitude')
    passenger_count = data.get('passengers')

    vehicle_data["latitude"] = latitude
    vehicle_data["longitude"] = longitude
    vehicle_data["passenger_count"] = passenger_count

    return 'Data received and updated', 200

@app.route('/get_vehicle_data', methods=['GET'])
def get_vehicle_data():
    response = {
        "latitude": vehicle_data["latitude"],
        "longitude": vehicle_data["longitude"],
        "passenger_count": vehicle_data["passenger_count"]
    }
    return jsonify(response), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

