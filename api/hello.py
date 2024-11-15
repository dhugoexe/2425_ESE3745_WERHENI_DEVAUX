from flask import Flask, render_template, jsonify, abort, request
import json

from serial_com import STM32Communication
app = Flask(__name__)

welcome = "Welcome to 3ESE API!"

temperatures = []
pressures = []
scale = 1

@app.route('/api/welcome/', methods=['GET', 'POST', 'DELETE'])
def api_welcome():
    global welcome

    if request.method == 'GET':
        response = jsonify({'data': welcome})
        response.headers['Content-Type'] = 'application/json'
        return response

    elif request.method == 'POST':
        received_data = request.get_json()
        if 'sentence' not in received_data:
            abort(400)
        welcome = received_data['sentence']
        return jsonify({'data': welcome})

    elif request.method == 'DELETE':
        welcome = ""
        return jsonify({'data': welcome})
@app.route('/api/welcome/<int:index>', methods=['GET', 'PUT', 'PATCH', 'DELETE'])
def api_welcome_index(index):
    global welcome

    if index >= len(welcome):
        abort(404)

    if request.method == 'GET':
        response = jsonify({'data': welcome[index], 'index': index})
        response.headers['Content-Type'] = 'application/json'
        return response

    elif request.method == 'PUT':
        received_data = request.get_json()
        if 'word' not in received_data:
            abort(400)
        new_word = received_data['word']
        welcome = welcome[:index] + new_word + welcome[index:]
        return jsonify({'data': welcome, 'index': index})

    elif request.method == 'PATCH':
        received_data = request.get_json()
        if 'letter' not in received_data or len(received_data['letter']) != 1:
            abort(400)
        welcome = welcome[:index] + received_data['letter'] + welcome[index + 1:]
        return jsonify({'data': welcome, 'index': index})

    elif request.method == 'DELETE':
        welcome = welcome[:index] + welcome[index + 1:]
        return jsonify({'data': welcome, 'index': index})


@app.route('/temp/', methods=['POST'])
def create_temperature():
    temp = request.json.get('temperature')
    temperatures.append(temp)
    return jsonify({'temperature': temp}), 201

@app.route('/pres/', methods=['POST'])
def create_pressure():
    pres = request.json.get('pressure')
    pressures.append(pres)
    return jsonify({'pressure': pres}), 201

@app.route('/temp/', methods=['GET'])
def get_temperatures():
    stm = STM32Communication()
    stm.send_command('GET_T')
    response = stm.read_response()
    return jsonify({'temperature': response})

@app.route('/temp/<int:temp_id>', methods=['GET'])
def get_temperature(temp_id):
    stm = STM32Communication()
    stm.send_command('GET_T')
    response = stm.read_response()
    return jsonify({'temperature': response})


@app.route('/pres/', methods=['GET'])
def get_pressures():
    return jsonify({'pressures': pressures})

@app.route('/pres/<int:pres_id>', methods=['GET'])
def get_pressure(pres_id):
    return jsonify({'pressure': pressures[pres_id]})


@app.route('/scale/', methods=['GET'])
def get_scale():
    return jsonify({'scale': scale})

@app.route('/angle/', methods=['GET'])
def get_angle():
    angle = temperatures[-1] * scale
    return jsonify({'angle': angle})

@app.route('/scale/<int:temp_id>', methods=['POST'])
def update_scale(temp_id):
    return jsonify({'scale': scale})


@app.route('/temp/<int:temp_id>', methods=['DELETE'])
def delete_temperature(temp_id):
    if 0 <= temp_id < len(temperatures):
        deleted_temp = temperatures.pop(temp_id)
        return jsonify({'deleted': deleted_temp})
    return jsonify({'error': 'Not found'}), 404

@app.route('/pres/<int:pres_id>', methods=['DELETE'])
def delete_pressure(pres_id):
    if 0 <= pres_id < len(pressures):
        deleted_pres = pressures.pop(pres_id)
        return jsonify({'deleted': deleted_pres})
    return jsonify({'error': 'Not found'}), 404

@app.route('/api/status/')
def api_get_status():
    return jsonify({'status': 'ok'})

@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404

@app.errorhandler(400)
def bad_request(error):
    return jsonify({'error': 'Bad Request'}), 400


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)