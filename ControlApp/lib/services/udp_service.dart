import 'dart:io';
import 'package:udp/udp.dart';

class UDPService {
  final String espIp = "192.168.4.1";
  final int espPort = 4210;
  late UDP sender;

  UDPService() {
    _initUDP();
  }

  Future<void> _initUDP() async {
    sender = await UDP.bind(Endpoint.any());
  }

  Future<void> sendParameter(String path, String value) async {
    final message = "$path=$value";
    await sender.send(message.codeUnits, Endpoint.unicast(InternetAddress(espIp), port: Port(espPort)));
  }

  Future<void> sendControlData(double throttle, double yaw, double pitch, double roll) async {
    final String data = "${throttle.toStringAsFixed(2)},${yaw.toStringAsFixed(2)},${pitch.toStringAsFixed(2)},${roll.toStringAsFixed(2)}";
    await sender.send(data.codeUnits, Endpoint.unicast(InternetAddress(espIp), port: Port(espPort)));
  }
}
