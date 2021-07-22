FROM flaskbase

COPY requirements.txt requirements.txt
RUN pip3 --no-cache-dir install -r requirements.txt                                                                            

WORKDIR /app

COPY . /app

EXPOSE 5000

ENV FLASK_DEBUG=0

ENTRYPOINT  ["python3"]

CMD ["application.py"]
