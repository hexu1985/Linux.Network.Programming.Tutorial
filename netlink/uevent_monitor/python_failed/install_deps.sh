python3 -m venv venv
source venv/bin/activate
PIP_SOURCE="https://pypi.tuna.tsinghua.edu.cn/simple"
pip install -i ${PIP_SOURCE} --upgrade pip
pip install -i ${PIP_SOURCE} pyinstaller
