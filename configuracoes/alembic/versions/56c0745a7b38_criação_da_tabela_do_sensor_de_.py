"""criação da tabela do sensor de temperatura

Revision ID: 56c0745a7b38
Revises: 
Create Date: 2026-07-21 11:09:50.761222

"""
from typing import Sequence, Union

from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision: str = '56c0745a7b38'
down_revision: Union[str, Sequence[str], None] = None
branch_labels: Union[str, Sequence[str], None] = None
depends_on: Union[str, Sequence[str], None] = None


def upgrade() -> None:
    """Upgrade schema."""
    op.create_table(
        "dados_sensor",
        sa.Column("ID", sa.Integer, sa.Identity(), primary_key=True),
        sa.Column("temperatura", sa.Double(2), nullable=False),
        sa.Column("registro", sa.TIMESTAMP, nullable=False),
        sa.PrimaryKeyConstraint("ID")
    );
    pass


def downgrade() -> None:
    """Downgrade schema."""
    op.drop_table("dados_sensor");
    pass
