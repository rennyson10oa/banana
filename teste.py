import pymongo

uri = "mongodb+srv://rennyson:rennyson100a@helpy.sllrgwg.mongodb.net/?retryWrites=true&w=majority"

try:
    cliente = pymongo.MongoClient(uri)

    # Exemplo de seleção de um banco de dados
    db = cliente.get_database("seu_banco_de_dados")

    # Exemplo de seleção de uma coleção
    colecao = db.get_collection("sua_colecao")

    # Fazer as operações desejadas no banco de dados e coleção aqui.

    cliente.close()
    
    print("Conexão bem-sucedida com o MongoDB remoto!")

except pymongo.errors.ConnectionFailure as e:
    print(f"Erro ao conectar ao MongoDB: {e}")
